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
		if (size.x <= 0 || size.y <= 0)
		{
			TS_CORE_ERROR("Failed to create texture, invalid size ({},{})", size.x, size.y);
			return false;
		}

		textureFormat = format;
		initializeTexture(size);

		int width = static_cast<GLsizei>(size.x);
		int height = static_cast<GLsizei>(size.y);

		// Find the format mapping and generate texture
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
		// Initial validation checks before doing any OpenGL calls
		if (width <= 0 || height <= 0) 
		{
			TS_CORE_ERROR("Invalid texture size: width = {}, height = {}", width, height);
			TS_CORE_ASSERT(false, "Texture dimensions must be greater than zero.");
			return;
		}

		// Log texture format and type information for debugging
		TS_CORE_INFO("Generating texture with internalFormat: {}, format: {}, type: {}, width: {}, height: {}",
			internalFormat, glFormat, glType, width, height);

		// Create texture image
		GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, glFormat, glType, data));
		GLenum glError = glGetError();
		if (glError != GL_NO_ERROR)
		{
			TS_CORE_ERROR("OpenGL error after glTexImage2D call. Error code: {}", glError);
			return;
		}

		// Set texture wrapping and filtering options
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, smooth ? GL_LINEAR : GL_NEAREST));

		// Check for any errors after setting parameters
		glError = glGetError();
		if (glError != GL_NO_ERROR) {
			TS_CORE_ERROR("OpenGL error after glTexParameteri call for wrap and filter settings. Error code: {}", glError);
			return;
		}

		// Handle mipmaps
		if (mipmapped) {
			GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, smooth ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST));
			GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
			hasMipmap = true;
			TS_CORE_INFO("Generated mipmaps for texture.");
		}
		else {
			GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, smooth ? GL_LINEAR : GL_NEAREST));
			hasMipmap = false;
			TS_CORE_INFO("No mipmaps generated for texture.");
		}

		// Check for OpenGL error after generating mipmaps (or not)
		glError = glGetError();
		if (glError != GL_NO_ERROR) {
			TS_CORE_ERROR("OpenGL error after glGenerateMipmap or glTexParameteri for mipmaps. Error code: {}", glError);
			return;
		}

		// Set the number of channels based on the format
		numChannels = (glFormat == GL_RGBA) ? 4 : (glFormat == GL_RGB) ? 3 : (glFormat == GL_RED) ? 1 : 0;

		if (numChannels == 0) {
			TS_CORE_WARN("Unknown number of channels for texture format: {}", glFormat);
		}

		// Log and generate raw data if needed
		if (numChannels != 0 && keepRaw) {
			TS_CORE_INFO("Generating raw data for texture (channels: {}).", numChannels);
			generateRawData(glType, width, height, numChannels, data);
		}
		else if (numChannels == 0) {
			TS_CORE_WARN("Couldn't create raw data due to numChannels being 0.");
		}

		// Final check to ensure no OpenGL errors
		glError = glGetError();
		if (glError != GL_NO_ERROR) {
			TS_CORE_ERROR("OpenGL error after texture data generation. Error code: {}", glError);
			return;
		}

		// Unbind the texture at the end
		GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
		glError = glGetError();
		if (glError != GL_NO_ERROR) {
			TS_CORE_ERROR("OpenGL error after glBindTexture(GL_TEXTURE_2D, 0). Error code: {}", glError);
			return;
		}

		TS_CORE_INFO("Texture data generation complete.");
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

	TextureFormat findTextureFormat(int numChannels, TextureBitDepth bitDepth)
	{
		switch (bitDepth)
		{
		case TextureBitDepth::BitDepth8:
			switch (numChannels)
			{
			case 1: return TextureFormat::R8;
			case 2: return TextureFormat::RG8;
			case 3: return TextureFormat::RGB8;
			case 4: return TextureFormat::RGBA8;
			default:
				TS_CORE_WARN("Unknown numChannels {} for bitDepth 8", numChannels);
				return TextureFormat::Unknown;
			}

		case TextureBitDepth::BitDepth16:
			switch (numChannels)
			{
			case 1: return TextureFormat::R16;
			case 2: return TextureFormat::RG16;
			case 3: return TextureFormat::RGB16;
			case 4: return TextureFormat::RGBA16;
			default:
				TS_CORE_WARN("Unknown numChannels {} for bitDepth 16", numChannels);
				return TextureFormat::Unknown;
			}

		case TextureBitDepth::BitDepth32:
			switch (numChannels)
			{
			case 1: return TextureFormat::R32F;
			case 2: return TextureFormat::RG32F;
			case 3: return TextureFormat::RGB32F;
			case 4: return TextureFormat::RGBA32F;
			default:
				TS_CORE_WARN("Unknown numChannels {} for bitDepth 32", numChannels);
				return TextureFormat::Unknown;
			}

		default:
			TS_CORE_WARN("Unknown bitDepth {}", static_cast<int>(bitDepth));
			return TextureFormat::Unknown;
		}
	}


	bool GLTexture2D::loadFromFile(const std::filesystem::path& filename, bool keepRaw, TextureBitDepth bitDepth)
	{
		int width, height, numChannels = 0;
		const std::string adapter = filename.string();
		const char* path = adapter.c_str();

		filepath = filename;

		if(bitDepth == TextureBitDepth::BitDepth8)
			pixels = stbi_load(path, &width, &height, &numChannels, STBI_default);
		else if(bitDepth == TextureBitDepth::BitDepth16)
			pixels16 = stbi_load_16(path, &width, &height, &numChannels, STBI_default);
		else if(bitDepth == TextureBitDepth::BitDepth32)
			pixels32 = stbi_loadf(path, &width, &height, &numChannels, STBI_default);


		if (!pixels && !pixels16)
			TS_CORE_ERROR("error: in GLTexture2D::loadFromFile, unable to load from path: {}\n", path);

		Thsan::TextureFormat data_format = findTextureFormat(numChannels, bitDepth);

		if(pixels || pixels16)
		{
			size.x = static_cast<uint32_t>(width);
			size.y = static_cast<uint32_t>(height);
			this->numChannels = static_cast<uint32_t>(numChannels);

			if(bitDepth == TextureBitDepth::BitDepth8)
				generate(size, data_format, pixels, keepRaw, true, false);
			else if(bitDepth == TextureBitDepth::BitDepth16)
				generate(size, data_format, pixels16, keepRaw, false, false);
			else if(bitDepth == TextureBitDepth::BitDepth32)
				generate(size, data_format, pixels16, keepRaw, false, false);


			TS_CORE_TRACE("load {}-channel texture from {}", numChannels, path);
		}
		else
		{
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
		// 8-bit integer formats (STBI supported)
		{TextureFormat::R8,    {GL_R8, GL_RED, GL_UNSIGNED_BYTE}},
		{TextureFormat::RG8,   {GL_RG8, GL_RG, GL_UNSIGNED_BYTE}},
		{TextureFormat::RGB8,  {GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE}},
		{TextureFormat::RGBA8, {GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE}},

		// 16-bit integer formats (STBI supported)
		{TextureFormat::R16,   {GL_R16, GL_RED, GL_UNSIGNED_SHORT}},
		{TextureFormat::RG16,  {GL_RG16, GL_RG, GL_UNSIGNED_SHORT}},
		{TextureFormat::RGB16, {GL_RGB16, GL_RGB, GL_UNSIGNED_SHORT}},
		{TextureFormat::RGBA16,{GL_RGBA16, GL_RGBA, GL_UNSIGNED_SHORT}},

		// 16-bit floating-point (GL_HALF_FLOAT)
		{TextureFormat::R16F,   {GL_R16F, GL_RED, GL_HALF_FLOAT}},
		{TextureFormat::RG16F,  {GL_RG16F, GL_RG, GL_HALF_FLOAT}},
		{TextureFormat::RGB16F, {GL_RGB16F, GL_RGB, GL_HALF_FLOAT}},
		{TextureFormat::RGBA16F,{GL_RGBA16F, GL_RGBA, GL_HALF_FLOAT}},

		// 32-bit floating-point (HDR images via STBI)
		{TextureFormat::R32F,   {GL_R32F, GL_RED, GL_FLOAT}},
		{TextureFormat::RG32F,  {GL_RG32F, GL_RG, GL_FLOAT}},
		{TextureFormat::RGB32F, {GL_RGB32F, GL_RGB, GL_FLOAT}},
		{TextureFormat::RGBA32F,{GL_RGBA32F, GL_RGBA, GL_FLOAT}}
	};

}
