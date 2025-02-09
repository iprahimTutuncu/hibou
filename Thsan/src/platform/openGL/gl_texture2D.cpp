#include "pch.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "platform/openGL/gl_texture2D.h"
#include "platform/openGL/gl_helper.h"
#include "thsan/log.h"

namespace Thsan {
	GLTexture2D::GLTexture2D():
		size{0,0},
		numChannels(0),
		gl_texture_id(0),
		pixelsFlipped(false),
		fboAttachment(false),
		hasMipmap(false),
		pixels(nullptr)
	{
		repeated = true;
		smooth = false;
	}

	GLTexture2D::~GLTexture2D()
	{
		stbi_image_free(pixels);
	}

	bool GLTexture2D::isValid() const
	{
		return false;
	}

	bool GLTexture2D::generate(const glm::uvec2& size, TextureFormat format, const void* data, bool keepRaw, bool mipmapped, bool smooth)
	{
		if (size.x <= 0 || size.y <= 0){
			TS_CORE_ERROR("Failed to create texture, invalid size ({},{})", size.x, size.y);
			return false;
		}

		textureFormat = format;
		initializeTexture(size);

		int width = static_cast<GLsizei>(size.x);
		int height = static_cast<GLsizei>(size.y);

		auto it = FormatMapping.find(format);
		if (it != FormatMapping.end()) {
			auto& [internalFormat, glFormat, glType] = it->second;
			generateTextureData(internalFormat, glFormat, glType, width, height, data, mipmapped, smooth, keepRaw);
		}

		return true;
	}

	void GLTexture2D::initializeTexture(const glm::uvec2& size)
	{
		this->size = size;
		pixelsFlipped = false;
		fboAttachment = false;

		if (!gl_texture_id){
			GLuint texture;
			GL_CHECK(glGenTextures(1, &texture));
			gl_texture_id = texture;
		}

		GL_CHECK(glBindTexture(GL_TEXTURE_2D, gl_texture_id));
	}

	void GLTexture2D::generateTextureData(GLenum internalFormat, GLenum glFormat, GLenum glType, int width, int height, const void* data, bool mipmapped, bool smooth, bool keepRaw)
	{
		GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, glFormat, glType, data));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, smooth ? GL_LINEAR : GL_NEAREST));

		if (mipmapped) {
			GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, smooth ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST));
			GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
			hasMipmap = true;
		}
		else {
			GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, smooth ? GL_LINEAR : GL_NEAREST));
			hasMipmap = false;
		}

		numChannels = glFormat == GL_RGBA ? 4 : glFormat == GL_RGB ? 3 : glFormat == GL_RED ? 1 : 0;
		if (numChannels != 0 && keepRaw)
			generateRawData(glType, width, height, numChannels, data);
		
		else if (numChannels == 0)
			TS_CORE_WARN("couldn't create raw data from within GLTexture2D::create() method due to numChannels being 0");
		

		GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
	}

	void GLTexture2D::generateRawData(GLenum glType, int width, int height, int numChannels, const void* data)
	{
		const std::size_t numPixels = width * height * numChannels;
		rawSize = numPixels;

		if (rawData)
			delete[] rawData;
		rawData = new float[numPixels];

		if (glType == GL_FLOAT || glType == GL_HALF_FLOAT) {
			const float* fData = static_cast<const float*>(data);
			for (std::size_t i = 0; i < numPixels; ++i)
				rawData[i] = fData[i];
		}
		else {
			const unsigned char* uData = static_cast<const unsigned char*>(data);
			for (std::size_t i = 0; i < numPixels; ++i)
				rawData[i] = uData[i] / 255.0f;
		}
	}


	bool GLTexture2D::loadFromFile(const std::filesystem::path& filename, bool keepRaw)
	{
		int width, height, numChannels = 0;
		const std::string adapter = filename.string();
		const char* path = adapter.c_str();

		filepath = filename;

		pixels = stbi_load(path, &width, &height, &numChannels, STBI_default);

		if (!pixels)
			TS_CORE_ERROR("error: in GLTexture2D::loadFromFile, unable to load from path: {}\n", path);

		Thsan::TextureFormat data_format;
		switch (numChannels) {
		case 4:
			data_format = TextureFormat::RGBA8;
			break;
		case 3:
			data_format = TextureFormat::RGB8;
			break;
		case 1:
			data_format = TextureFormat::R32F;
			break;
		default:
			TS_CORE_ERROR("error: in GLTexture2D::loadFromFile, unsupported data format with {} channel\n", numChannels);
			break;
		}
		
		if(pixels){
			size.x = static_cast<uint32_t>(width);
			size.y = static_cast<uint32_t>(height);
			this->numChannels = static_cast<uint32_t>(numChannels);

			generate(size, data_format, pixels, keepRaw, true, false);
			TS_CORE_TRACE("load {}-channel texture from {}", numChannels, path);
		}
		else {
			TS_CORE_WARN("unable to load texture: {}", path);
			return false;
		}

		return true;
	}

	const std::filesystem::path& GLTexture2D::getFilePath() const
	{
		return filepath;
	}

	const float* GLTexture2D::getRawData() const
	{
		return rawData;
	}


	void GLTexture2D::bind(uint32_t slot)
	{
		TS_CORE_ASSERT(slot < 16, "calme toi avec les texture sample lol, je sais même pas si tous les hardware support autant.");
		GL_CHECK(glActiveTexture(GL_TEXTURE0 + slot));
		GL_CHECK(glBindTexture(GL_TEXTURE_2D, gl_texture_id));
	}
	glm::uvec2 GLTexture2D::getSize() const
	{
		return size;
	}
	uint32_t GLTexture2D::getWidth() const
	{
		return size.x;
	}
	uint32_t GLTexture2D::getHeight() const
	{
		return size.y;
	}

	const int GLTexture2D::getNumChannels() const
	{
		return numChannels;
	}

	uint32_t GLTexture2D::getSpecificRenderAPI_ID() const
	{
		return gl_texture_id;
	}
	TextureFormat GLTexture2D::getTextureFormat() const
	{
		return textureFormat;
	}

	void GLTexture2D::clear(float r, float g, float b, float a)
	{
		auto it = FormatMapping.find(textureFormat);

		TS_CORE_ASSERT(it != FormatMapping.end(), "In, GLTexture2D::clear, unsupported type file, can't clear it");

		GLenum internalFormat = std::get<0>(it->second);
		GLenum format = std::get<1>(it->second);
		GLenum type = std::get<2>(it->second);

		GL_CHECK(glBindTexture(GL_TEXTURE_2D, gl_texture_id));

		void* clearColorData = nullptr;
		float clearColorF[4] = { r, g, b, a };
		unsigned char clearColorUB[4] = {
			static_cast<unsigned char>(r * 255),
			static_cast<unsigned char>(g * 255),
			static_cast<unsigned char>(b * 255),
			static_cast<unsigned char>(a * 255)
		};

		if (type == GL_FLOAT)
			clearColorData = clearColorF;
		else if (type == GL_UNSIGNED_BYTE)
			clearColorData = clearColorUB;
		
		GL_CHECK(glClearTexImage(gl_texture_id, 0, format, type, clearColorData));
	}

	const std::unordered_map<TextureFormat, std::tuple<GLenum, GLenum, GLenum>> GLTexture2D::FormatMapping = {
	{TextureFormat::RGBA8, {GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE}},
	{TextureFormat::R32F, {GL_R32F, GL_RED, GL_FLOAT}},
	{TextureFormat::RGB16F, {GL_RGB16F, GL_RGB, GL_HALF_FLOAT}},
	{TextureFormat::RGBA16F, {GL_RGBA16F, GL_RGBA, GL_HALF_FLOAT}},
	{TextureFormat::RGB32F, {GL_RGB32F, GL_RGB, GL_FLOAT}},
	{TextureFormat::RGBA32F, {GL_RGBA32F, GL_RGBA, GL_FLOAT}},
	{TextureFormat::RGB8, {GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE}}
	};
}
