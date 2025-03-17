#pragma once
#include "thsan/core.h"
#include <fstream>
#include <glm/ext/vector_uint2.hpp>

namespace Thsan {

	enum class THSAN_API TextureFormat
	{
		Unknown,
		// 8-bit per channel (STBI supported)
		R8,       // GL_R8, GL_RED, GL_UNSIGNED_BYTE
		RG8,      // GL_RG8, GL_RG, GL_UNSIGNED_BYTE
		RGB8,     // GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE
		RGBA8,    // GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE

		// 16-bit per channel (STBI supported via stbi_load_16)
		R16,      // GL_R16UI, GL_RED, GL_UNSIGNED_SHORT
		RG16,     // GL_RG16UI, GL_RG, GL_UNSIGNED_SHORT
		RGB16,    // GL_RGB16UI, GL_RGB, GL_UNSIGNED_SHORT
		RGBA16,   // GL_RGBA16UI, GL_RGBA, GL_UNSIGNED_SHORT

		// 16-bit floating-point (GL_HALF_FLOAT)
		R16F,     // GL_R16F, GL_RED, GL_HALF_FLOAT
		RG16F,    // GL_RG16F, GL_RG, GL_HALF_FLOAT
		RGB16F,   // GL_RGB16F, GL_RGB, GL_HALF_FLOAT
		RGBA16F,  // GL_RGBA16F, GL_RGBA, GL_HALF_FLOAT

		// 32-bit floating-point (HDR images via stbi_loadf)
		R32F,     // GL_R32F, GL_RED, GL_FLOAT
		RG32F,    // GL_RG32F, GL_RG, GL_FLOAT
		RGB32F,   // GL_RGB32F, GL_RGB, GL_FLOAT
		RGBA32F   // GL_RGBA32F, GL_RGBA, GL_FLOAT
	};

	enum class THSAN_API TextureBitDepth
	{
		BitDepth8,
		BitDepth16,
		BitDepth32
	};

	class THSAN_API Texture {

	public:
		Texture();
		Texture(const Texture& copy) = delete;
		bool operator==(const Texture& other);
		virtual ~Texture() = default;

		virtual bool isValid() const = 0;
		virtual [[nodiscard]] bool generate(const glm::uvec2& size, TextureFormat format, const void* data = nullptr, bool keepRaw = false, bool mipmapped = false, bool smooth = false) = 0;

		virtual bool loadFromFile(const std::filesystem::path& filename, bool keepRaw, TextureBitDepth bitDepth) = 0;
		virtual const std::filesystem::path& getFilePath() const = 0;

		// todo
		//virtual Image copyToImage() const = 0;
		//virtual [[nodiscard]] bool loadFromImage(const Image& image, const glm::vec4& area = glm::vec4()) = 0;
		virtual [[nodiscard]] const float* getRawData() const = 0;
		virtual const int getNumChannels() const = 0;

		virtual bool isSmooth() const;
		virtual bool isRepeated() const;

		//virtual void enableSmooth(bool smooth) = 0;
		//virtual void enableRepeated(bool repeated) = 0;

		//virtual [[nodiscard]] bool generateMipmap() = 0;
		virtual void bind(uint32_t slot) = 0;

		uint32_t getID() const { return id; }
		virtual uint32_t getSpecificRenderAPI_ID() const = 0;

		virtual TextureFormat getTextureFormat() const = 0;
		virtual void clear(float r = 0.f, float g = 0.f, float b = 0.f, float a = 1.f) = 0;
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

		virtual glm::uvec2 getSize() const = 0;
		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;
		virtual void clear(float r = 0.f, float g = 0.f, float b = 0.f, float a = 1.f) = 0;

		static std::shared_ptr<Texture2D> create();
	};
}

