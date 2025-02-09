#include "pch.h"
#include "platform/openGL/gl_compute_shader.h"
#include "platform/openGL/gl_mesh.h"
#include "platform/openGL/gl_texture2D.h"
#include "platform/openGL/gl_helper.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <thsan/graphics/texture.h>

namespace Thsan {

	GLComputeShader::GLComputeShader(const std::string& cs_path)
	{
		program_id = glCreateProgram();
		int status = GL_FALSE;
		uint32_t cs_id = glCreateShader(GL_COMPUTE_SHADER);

		{
			char error_log[512];
			std::string text = readFile(cs_path);
			const GLchar* glSource = text.c_str();
			GL_CHECK(glShaderSource(cs_id, 1, &glSource, nullptr));
			GL_CHECK(glCompileShader(cs_id));
			GL_CHECK(glGetShaderiv(cs_id, GL_COMPILE_STATUS, &status));

			if (status != GL_TRUE) {
				GL_CHECK(glGetShaderInfoLog(cs_id, sizeof(error_log), nullptr, error_log));
				TS_CORE_ERROR("\nCompute shader compilation error:\n{}", error_log);
			}

			GL_CHECK(glAttachShader(program_id, cs_id));
		}

		TS_CORE_ASSERT(status == GL_TRUE, "ERROR IN COMPUTE SHADER");

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

		GL_CHECK(glDeleteShader(cs_id));
	}

	GLComputeShader::~GLComputeShader()
	{
		GL_CHECK(glUseProgram(0));
		GL_CHECK(glDeleteProgram(program_id));
	}

	void GLComputeShader::use()
	{
		GL_CHECK(glUseProgram(program_id));
	}

	void GLComputeShader::dispatch(unsigned int x, unsigned int y, unsigned int z)
	{
		for (int i = 0; i < uniform_texture_ids.size(); i++) {
			auto& value = uniform_texture_ids[i];
			std::shared_ptr<Texture2D> tex2D = value.first.lock();
			tex2D->bind(i);
		}

		GL_CHECK(glDispatchCompute(x, y, z));
	}

	void GLComputeShader::wait()
	{
		GL_CHECK(glMemoryBarrier(GL_ALL_BARRIER_BITS));
	}

	void GLComputeShader::setBool(const std::string& name, bool value)
	{
		GL_CHECK(glUseProgram(program_id));
		GL_CHECK(glUniform1i(getUniformId(name), static_cast<int>(value)));
	}

	void GLComputeShader::setInt(const std::string& name, int value)
	{
		GL_CHECK(glUseProgram(program_id));
		GL_CHECK(glUniform1i(getUniformId(name), value));
	}

	void GLComputeShader::setFloat(const std::string& name, float value)
	{
		GL_CHECK(glUseProgram(program_id));
		GL_CHECK(glUniform1f(getUniformId(name), value));
	}

	void GLComputeShader::setMat3(const std::string& name, glm::mat3 value)
	{
		GL_CHECK(glUseProgram(program_id));
		GL_CHECK(glUniformMatrix3fv(getUniformId(name), 1, GL_FALSE, glm::value_ptr(value)));
	}

	void GLComputeShader::setMat4(const std::string& name, glm::mat4 value)
	{
		GL_CHECK(glUseProgram(program_id));
		GL_CHECK(glUniformMatrix4fv(getUniformId(name), 1, GL_FALSE, glm::value_ptr(value)));
	}

	void GLComputeShader::setVec4(const std::string& name, glm::vec4 value)
	{
		GL_CHECK(glUseProgram(program_id));
		GL_CHECK(glUniform4fv(getUniformId(name), 1, glm::value_ptr(value)));
	}

	void GLComputeShader::setVec2(const std::string& name, glm::vec2 value)
	{
		GL_CHECK(glUseProgram(program_id));
		GL_CHECK(glUniform2fv(getUniformId(name), 1, glm::value_ptr(value)));
	}

	void GLComputeShader::setVec3(const std::string& name, glm::vec3 value)
	{
		GL_CHECK(glUseProgram(program_id));
		GL_CHECK(glUniform3fv(getUniformId(name), 1, glm::value_ptr(value)));
	}
	void GLComputeShader::setTexture2D(const std::string& name, std::weak_ptr<Texture2D> tex2D)
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

	void GLComputeShader::setImage2D(std::weak_ptr<Texture2D> tex2D, int bindingPoint)
	{
		GLuint textureID = tex2D.lock()->getSpecificRenderAPI_ID();
		auto texture = tex2D.lock();

		auto it = GLTexture2D::FormatMapping.find(texture->getTextureFormat());
		if (it != GLTexture2D::FormatMapping.end()) {
			auto& [internalFormat, glFormat, glType] = it->second;
			GL_CHECK(glBindImageTexture(bindingPoint, textureID, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8));
		}
		else {
			TS_CORE_ERROR("GLComputeShader::setRWImage2D, tex2D textureFormat doesn't correspond to binding point");
		}
	}

	void GLComputeShader::setRWImage2D(std::weak_ptr<Texture2D> tex2D, int bindingPoint)
	{
		GLuint textureID = tex2D.lock()->getSpecificRenderAPI_ID();

		auto texture = tex2D.lock();

		auto it = GLTexture2D::FormatMapping.find(texture->getTextureFormat());
		if (it != GLTexture2D::FormatMapping.end()) {
			auto& [internalFormat, glFormat, glType] = it->second;

			TS_CORE_INFO("Internal Format: {}, GL Format: {}, GL Type: {}", internalFormat, glFormat, glType);

			// Log the binding details
			TS_CORE_INFO("Binding texture ID {} to image unit {} with internal format {}", textureID, bindingPoint, internalFormat);

			// Perform the texture binding
			GL_CHECK(glBindImageTexture(bindingPoint, textureID, 0, GL_FALSE, 0, GL_READ_WRITE, internalFormat));

			// Log success
			TS_CORE_INFO("Successfully bound texture ID {} to image unit {}", textureID, bindingPoint);
		}
		else {
			TS_CORE_ERROR("GLComputeShader::setRWImage2D, tex2D textureFormat doesn't correspond to binding point");
		}
	}

	void GLComputeShader::setRWMesh(std::weak_ptr<Mesh> mesh, int bindingPoint)
	{
		std::shared_ptr<GLMesh> tempMesh = std::static_pointer_cast<GLMesh>(mesh.lock());
		if (!tempMesh){
			return;
		}

		GL_CHECK(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, tempMesh->getVBO()));
		GL_CHECK(glUniform1ui(glGetUniformLocation(program_id, "vertexCount"), static_cast<GLuint>(tempMesh->getVertexCount())));
	}

	int GLComputeShader::getUniformId(const std::string& name)
	{
		if (uniform_ids.find(name) == uniform_ids.end())
			uniform_ids[name] = GL_CHECK(glGetUniformLocation(program_id, name.c_str()));

		return uniform_ids[name];
	}
}