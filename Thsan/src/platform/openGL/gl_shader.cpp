#include "pch.h"
#include "gl_shader.h"
#include "gl_helper.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

namespace Thsan {

	GLShader::GLShader(const std::string& vs_path, const std::string& fs_path)
	{
		program_id = glCreateProgram();
		int status = GL_FALSE;
		uint32_t vs_id = glCreateShader(GL_VERTEX_SHADER);

		{
			char error_log[512];
			std::string text = readFile(vs_path);
			const GLchar* glSource = text.c_str();
			GL_CHECK(glShaderSource(vs_id, 1, &glSource, nullptr));
			GL_CHECK(glCompileShader(vs_id));
			GL_CHECK(glGetShaderiv(vs_id, GL_COMPILE_STATUS, &status));

			if (status != GL_TRUE) {
				GL_CHECK(glGetShaderInfoLog(vs_id, sizeof(error_log), nullptr, error_log));
				TS_CORE_ERROR("\nVertex shader compilation error:\n{}", error_log);
			}

			GL_CHECK(glAttachShader(program_id, vs_id));
		}

		TS_CORE_ASSERT(status == GL_TRUE, "ERROR IN VERTEX SHADER");

		uint32_t fs_id = glCreateShader(GL_FRAGMENT_SHADER);
		{
			status = GL_FALSE;
			char error_log[512];
			std::string text = readFile(fs_path);
			const GLchar* glSource = text.c_str();
			GL_CHECK(glShaderSource(fs_id, 1, &glSource, nullptr));
			GL_CHECK(glCompileShader(fs_id));
			GL_CHECK(glGetShaderiv(fs_id, GL_COMPILE_STATUS, &status));

			if (status != GL_TRUE) {
				GL_CHECK(glGetShaderInfoLog(fs_id, sizeof(error_log), nullptr, error_log));
				TS_CORE_ERROR("\nFragment shader compilation error :\n{}", error_log);
			}

			GL_CHECK(glAttachShader(program_id, fs_id));
		}

		TS_CORE_ASSERT(status == GL_TRUE, "ERROR IN FRAGMENT SHADER");

		if (status == GL_TRUE)
		{
			char error_log[512];
			GL_CHECK(glLinkProgram(program_id));
			GL_CHECK(glValidateProgram(program_id));

			GL_CHECK(glGetProgramiv(program_id, GL_LINK_STATUS, &status));
			if (status != GL_TRUE) {
				GL_CHECK(glGetProgramInfoLog(program_id, sizeof(error_log), nullptr, error_log));
				TS_CORE_ERROR("Program compilation error: {}\n", error_log);
				GL_CHECK(glDeleteProgram(program_id));
				program_id = -1;
			}
		}


		GL_CHECK(glDeleteShader(vs_id));
		GL_CHECK(glDeleteShader(fs_id));
	}

	GLShader::~GLShader()
	{
		GL_CHECK(glUseProgram(0));
		GL_CHECK(glDeleteProgram(program_id));
	}
	void GLShader::bind()
	{
		GL_CHECK(glUseProgram(program_id));
		for (int i = 0; i < uniform_texture_ids.size(); i++) {
			auto value = uniform_texture_ids[i];
			std::shared_ptr<Texture2D> tex2D = value.first.lock();
			tex2D->bind(i);
		}
	}
	void GLShader::unBind()
	{
		GL_CHECK(glUseProgram(0));
		texture_count = 0;
	}

	void GLShader::setUniformBool(const std::string& name, bool value)
	{
		GL_CHECK(glUseProgram(program_id));
		GL_CHECK(glUniform1i(getUniformId(name), static_cast<int>(value)));
	}

	void GLShader::setUniformInt(const std::string& name, int value)
	{
		GL_CHECK(glUseProgram(program_id));
		GL_CHECK(glUniform1i(getUniformId(name), value));
	}

	void GLShader::setUniformFloat(const std::string& name, float value)
	{
		GL_CHECK(glUseProgram(program_id));
		GL_CHECK(glUniform1f(getUniformId(name), value));
	}

	void GLShader::setMat3(const std::string& name, glm::mat3 value)
	{
		GL_CHECK(glUseProgram(program_id));
		GL_CHECK(glUniformMatrix3fv(getUniformId(name), 1, GL_FALSE, glm::value_ptr(value)));
	}

	void GLShader::setMat4(const std::string& name, glm::mat4 value)
	{
		GL_CHECK(glUseProgram(program_id));
		GL_CHECK(glUniformMatrix4fv(getUniformId(name), 1, GL_FALSE, glm::value_ptr(value)));
	}

	void GLShader::setVec4(const std::string& name, glm::vec4 value)
	{
		GL_CHECK(glUseProgram(program_id));
		GL_CHECK(glUniform4fv(getUniformId(name), 1, glm::value_ptr(value)));
	}

	void GLShader::setVec3(const std::string& name, glm::vec3 value)
	{
		GL_CHECK(glUseProgram(program_id));
		GL_CHECK(glUniform3fv(getUniformId(name), 1, glm::value_ptr(value)));
	}
	void GLShader::setTexture2D(const std::string& name, std::weak_ptr<Texture2D> tex2D)
	{

		int32_t texture_unif_Loc = getUniformId(name.c_str());

		for (auto& entry : uniform_texture_ids)
		{
			if (entry.second == texture_unif_Loc)
			{
				entry.first = tex2D;
				return;
			}
		}

		uniform_texture_ids.push_back(std::make_pair(tex2D, texture_unif_Loc));
		GL_CHECK(glUniform1i(texture_unif_Loc, texture_count++));
	}

	int GLShader::getUniformId(const std::string& name) 
	{
		if (uniform_ids.find(name) == uniform_ids.end())
			uniform_ids[name] = GL_CHECK(glGetUniformLocation(program_id, name.c_str()));

		return uniform_ids[name];
	}
}
