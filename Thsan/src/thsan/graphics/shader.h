#pragma once

#include "thsan/core.h"
#include "thsan/log.h"
#include "thsan/graphics/texture.h"

#include <string>
#include <glm/glm.hpp>
#include <tsm/tsm.h>

namespace Thsan {

	enum class default_shader_texture {
		white,
		checkerboard
	};

	class THSAN_API Shader {
	public:
		Shader() = default;
		virtual ~Shader() = default;

		virtual void bind() = 0;
		virtual void unBind() = 0;

		virtual void setUniformBool(const std::string& name, bool value) = 0;
		virtual void setUniformInt(const std::string& name, int value) = 0;
		virtual void setUniformFloat(const std::string& name, float value) = 0;

		virtual void setMat3(const std::string& name, glm::mat3 value) = 0;
		virtual void setMat4(const std::string& name, glm::mat4 value) = 0;

		virtual void setVec3(const std::string& name, glm::vec3 value) = 0;
		virtual void setVec4(const std::string& name, glm::vec4 value) = 0;

		virtual void setCamera(std::weak_ptr<tsm::AbstractCamera> camera);
		virtual void setTexture2D(const std::string& name, std::weak_ptr<Texture2D> tex2D) = 0;

	protected:
		std::string readFile(std::string path);

	};

	THSAN_API std::shared_ptr<Shader> create_shader(const std::string& vs_path, const std::string& fs_path);

}