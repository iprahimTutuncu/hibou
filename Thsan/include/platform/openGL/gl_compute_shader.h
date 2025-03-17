#pragma once
#include "thsan/graphics/compute_shader.h"

namespace Thsan {
	class Texture2D;
	class Mesh;

	class GLComputeShader : public ComputeShader {
	public:
		GLComputeShader(const std::string& cs_path);
		~GLComputeShader();

		virtual void use() override;
		virtual void dispatch(unsigned int x, unsigned int y, unsigned int z) override;
		virtual void wait() override;

		virtual void setBool(const std::string& name, bool value) override;
		virtual void setInt(const std::string& name, int value) override;
		virtual void setFloat(const std::string& name, float value) override;

		virtual void setFloatArray(const std::string& name, const float* values, size_t count) override;

		virtual void setMat3(const std::string& name, glm::mat3 value) override;
		virtual void setMat4(const std::string& name, glm::mat4 value) override;

		virtual void setVec2u(const std::string& name, glm::uvec2 value) override;
		virtual void setVec3u(const std::string& name, glm::uvec3 value) override;
		virtual void setVec4u(const std::string& name, glm::uvec4 value) override;

		virtual void setVec2i(const std::string& name, glm::ivec2 value) override;
		virtual void setVec3i(const std::string& name, glm::ivec3 value) override;
		virtual void setVec4i(const std::string& name, glm::ivec4 value) override;

		virtual void setVec2(const std::string& name, glm::vec2 value) override;
		virtual void setVec3(const std::string& name, glm::vec3 value) override;
		virtual void setVec4(const std::string& name, glm::vec4 value) override;

		virtual void setTexture2D(const std::string& name, std::weak_ptr<Texture2D> tex2D) override;
		virtual void setImage2D(std::weak_ptr<Texture2D> tex2D, int bindingPoint) override;

		virtual void setRWImage2D(std::weak_ptr<Texture2D> tex2D, int bindingPoint) override;

		//to delete, might not actually work. Everything is a sprite anyway, might not need it
		virtual void setRWMesh(std::weak_ptr<Mesh> mesh, int bindingPoint) override;

	private:
		int getUniformId(const std::string& name);

		std::unordered_map<std::string, int> uniform_ids;
		std::vector<std::pair<std::weak_ptr<Texture2D>, int32_t>> uniform_texture_ids;
		uint32_t program_id;
		uint32_t texture_count{ 0 };

	};
}
