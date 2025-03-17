#include "pch.h"
#include "platform/openGL/gl_compute_shader.h"
#include "platform/openGL/gl_mesh.h"
#include "platform/openGL/gl_texture2D.h"
#include "platform/openGL/gl_helper.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <thsan/graphics/texture.h>
#include <thsan/log.h>

namespace Thsan {
	GLComputeShader::GLComputeShader(const std::string& cs_path)
	{
		program_id = glCreateProgram();
		uint32_t cs_id = glCreateShader(GL_COMPUTE_SHADER);
		int status = GL_FALSE;

		// Read shader source
		std::string text = readFile(cs_path);
		if (text.empty()) {
			TS_CORE_ERROR("Failed to read compute shader file: {}", cs_path);
			TS_CORE_ASSERT(false, "Shader source is empty or failed to load.");
			return;
		}

		const GLchar* glSource = text.c_str();
		GL_CHECK(glShaderSource(cs_id, 1, &glSource, nullptr));
		GL_CHECK(glCompileShader(cs_id));
		GL_CHECK(glGetShaderiv(cs_id, GL_COMPILE_STATUS, &status));

		if (status != GL_TRUE) {
			std::vector<char> error_log(1024);
			GL_CHECK(glGetShaderInfoLog(cs_id, error_log.size(), nullptr, error_log.data()));
			TS_CORE_ERROR("Compute shader compilation error: {}\nShader source:\n{}", error_log.data(), text);
			TS_CORE_ASSERT(false, "Shader compilation failed.");
			GL_CHECK(glDeleteShader(cs_id));
			return;
		}

		GL_CHECK(glAttachShader(program_id, cs_id));
		GL_CHECK(glLinkProgram(program_id));
		GL_CHECK(glGetProgramiv(program_id, GL_LINK_STATUS, &status));

		if (status != GL_TRUE) {
			std::vector<char> error_log(1024);
			GL_CHECK(glGetProgramInfoLog(program_id, error_log.size(), nullptr, error_log.data()));
			TS_CORE_ERROR("Program linking error: {}\nShader source:\n{}", error_log.data(), text);
			TS_CORE_ASSERT(false, "Program linking failed.");
			GL_CHECK(glDeleteProgram(program_id));
			GL_CHECK(glDeleteShader(cs_id));
			program_id = 0;
			return;
		}

		// Cleanup
		GL_CHECK(glDeleteShader(cs_id));
		TS_CORE_INFO("Compute shader loaded and program linked successfully.");
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
		// Log the dispatch dimensions for debugging
		TS_CORE_INFO("Dispatching compute shader with dimensions: x = {}, y = {}, z = {}", x, y, z);

		// Iterate through the texture bindings and log them
		for (int i = 0; i < uniform_texture_ids.size(); i++) {
			auto& value = uniform_texture_ids[i];
			std::shared_ptr<Texture2D> tex2D = value.first.lock();

			if (!tex2D) 
			{
				TS_CORE_ERROR("Failed to lock texture at index {}. Texture is nullptr.", i);
				continue;
			}

			// Bind the texture and log any OpenGL errors
			tex2D->bind(i);

			GLenum glError = glGetError();
			if (glError != GL_NO_ERROR)
			{
				TS_CORE_ERROR("OpenGL Error: {} occurred while binding texture {} at index {}.", glError, tex2D->getID(), i);
				TS_CORE_ASSERT(false, "Error while binding texture in compute shader.");
			}
		}

		// Dispatch the compute shader
		GL_CHECK(glDispatchCompute(x, y, z));

		// Check for OpenGL errors after dispatching
		GLenum glError = glGetError();
		if (glError != GL_NO_ERROR) {
			TS_CORE_ERROR("OpenGL Error: {} occurred after glDispatchCompute({}, {}, {}).", glError, x, y, z);
			TS_CORE_ASSERT(false, "OpenGL Error after compute shader dispatch.");
		}

		TS_CORE_INFO("Compute shader dispatched successfully with dimensions: x = {}, y = {}, z = {}", x, y, z);
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

	void GLComputeShader::setFloatArray(const std::string& name, const float* values, size_t count)
	{
		GL_CHECK(glUseProgram(program_id));
		GL_CHECK(glUniform1fv(getUniformId(name), static_cast<GLsizei>(count), values));
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

	void GLComputeShader::setVec4u(const std::string& name, glm::uvec4 value)
	{
		GL_CHECK(glUseProgram(program_id));
		GL_CHECK(glUniform4uiv(getUniformId(name), 1, glm::value_ptr(value)));
	}

	void GLComputeShader::setVec2u(const std::string& name, glm::uvec2 value)
	{
		GL_CHECK(glUseProgram(program_id));
		GL_CHECK(glUniform2uiv(getUniformId(name), 1, glm::value_ptr(value)));
	}

	void GLComputeShader::setVec3u(const std::string& name, glm::uvec3 value)
	{
		GL_CHECK(glUseProgram(program_id));
		GL_CHECK(glUniform3uiv(getUniformId(name), 1, glm::value_ptr(value)));
	}

	void GLComputeShader::setVec4i(const std::string& name, glm::ivec4 value)
	{
		GL_CHECK(glUseProgram(program_id));
		GL_CHECK(glUniform4iv(getUniformId(name), 1, glm::value_ptr(value)));
	}

	void GLComputeShader::setVec2i(const std::string& name, glm::ivec2 value)
	{
		GL_CHECK(glUseProgram(program_id));
		GL_CHECK(glUniform2iv(getUniformId(name), 1, glm::value_ptr(value)));
	}

	void GLComputeShader::setVec3i(const std::string& name, glm::ivec3 value)
	{
		GL_CHECK(glUseProgram(program_id));
		GL_CHECK(glUniform3iv(getUniformId(name), 1, glm::value_ptr(value)));
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
		auto texture = tex2D.lock();
		if (!texture) 
		{
			TS_CORE_ERROR("Failed to lock texture!");
			return;
		}

		GLuint textureID = texture->getSpecificRenderAPI_ID();

		auto it = GLTexture2D::FormatMapping.find(texture->getTextureFormat());
		if (it != GLTexture2D::FormatMapping.end()) 
		{
			auto& [internalFormat, glFormat, glType] = it->second;
			GL_CHECK(glBindImageTexture(bindingPoint, textureID, 0, GL_FALSE, 0, GL_READ_ONLY, internalFormat));
		}
		else
		{
			TS_CORE_ERROR("GLComputeShader::setImage2D, texture format not found in format mapping.");
		}
	}


	void GLComputeShader::setRWImage2D(std::weak_ptr<Texture2D> tex2D, int bindingPoint)
	{
		GLuint textureID = tex2D.lock()->getSpecificRenderAPI_ID();

		auto texture = tex2D.lock();

		auto it = GLTexture2D::FormatMapping.find(texture->getTextureFormat());
		if (it != GLTexture2D::FormatMapping.end())
		{
			auto& [internalFormat, glFormat, glType] = it->second;
			GL_CHECK(glBindImageTexture(bindingPoint, textureID, 0, GL_FALSE, 0, GL_READ_WRITE, internalFormat));

		}
		else 
		{
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