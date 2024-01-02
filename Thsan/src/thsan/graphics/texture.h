#pragma once
#include "thsan/core.h"
#include "thsan/system/vector.h"
#include <fstream>
#include <glm/ext/vector_uint2.hpp>

namespace Thsan {

	enum class TextureFormat
	{
		RGBA8,   // Example: GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE
		R32F,    // Example: GL_R32F, GL_RED, GL_FLOAT
		RGB16F,  // Example: GL_RGB16F, GL_RGB, GL_HALF_FLOAT
	};

	class THSAN_API Texture {

	public:
		Texture();
		Texture(const Texture& copy) = delete;
		bool operator==(const Texture& other);
		virtual ~Texture() = default;

		virtual bool isValid() const = 0;
		virtual [[nodiscard]] bool create(const vec2u& size) = 0;
		virtual [[nodiscard]] bool create(const glm::uvec2& size, TextureFormat format, const void* data = nullptr, bool mipmapped = false, bool smooth = false) = 0;

		virtual bool loadFromFile(const std::filesystem::path& filename) = 0;

		// todo
		//virtual Image copyToImage() const = 0;
		//virtual [[nodiscard]] bool loadFromImage(const Image& image, const glm::vec4& area = glm::vec4()) = 0;
		virtual [[nodiscard]] bool loadFromMemory(const float* data, vec2u size) = 0;
		
		virtual bool isSmooth() const;
		virtual bool isRepeated() const;

		//virtual void enableSmooth(bool smooth) = 0;
		//virtual void enableRepeated(bool repeated) = 0;

		//virtual [[nodiscard]] bool generateMipmap() = 0;
		virtual void bind(uint32_t slot) = 0;

		uint32_t getID() const { return id; }
		virtual uint32_t getSpecificRenderAPI_ID() const = 0;
	protected:
		bool smooth;
		bool repeated;
		uint32_t id;
    		
	};

	class THSAN_API Texture2D: public Texture {
	public:
		Texture2D() = default;
		virtual ~Texture2D() = default;

		Texture2D(const Texture2D& copy) = delete;

		virtual vec2u getSize() const = 0;
		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;
	private:
	};

	THSAN_API std::shared_ptr<Texture2D> create_texture2D();

}

