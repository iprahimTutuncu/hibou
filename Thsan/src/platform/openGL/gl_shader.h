#pragma once
#include "thsan/graphics/shader.h"

namespace Thsan {
	class Texture2D;

	class GLShader : public Shader{
	public:
		GLShader(const std::string& vs_path, const std::string& fs_path);
		~GLShader();

		void bind() override;
		void unBind() override;

		void setUniformBool(const std::string& name, bool value) override;
		void setUniformInt(const std::string& name, int value) override;
		void setUniformFloat(const std::string& name, float value) override;

		void setMat3(const std::string& name, glm::mat3 value) override;
		void setMat4(const std::string& name, glm::mat4 value) override;

		void setVec3(const std::string& name, glm::vec3 value) override;
		void setVec4(const std::string& name, glm::vec4 value) override;

		void setTexture2D(const std::string& name, std::weak_ptr<Texture2D> tex2D) override;

	private:
		int getUniformId(const std::string& name);

		std::unordered_map<std::string, int> uniform_ids;
		std::vector<std::pair<std::weak_ptr<Texture2D>, int32_t>> uniform_texture_ids;
		uint32_t program_id;
		uint32_t texture_count{0};
	};
}
