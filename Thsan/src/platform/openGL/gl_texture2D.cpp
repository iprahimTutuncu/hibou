#include "pch.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "gl_texture2D.h"
#include "gl_helper.h"
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

	bool GLTexture2D::create(const vec2u& size)
	{

        if ((size.x <= 0) || (size.y <= 0))
        {
            TS_CORE_ERROR("Failed to create texture, invalid size ({}, {})", size.x, size.y);
            return false;
        }

        // All the validity checks passed, we can store the new texture settings
		this->size = {size.x, size.y};
        pixelsFlipped = false;
        fboAttachment = false;

        if (!gl_texture_id)
        {
            GLuint texture;
            GL_CHECK(glGenTextures(1, &texture));
            gl_texture_id = texture;
        }

        // Initialize the texture
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, gl_texture_id));

        int width = static_cast<GLsizei>(size.x);
        int height = static_cast<GLsizei>(size.y);
        GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, smooth ? GL_LINEAR : GL_NEAREST));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, smooth ? GL_LINEAR : GL_NEAREST));

		hasMipmap = false;

        return true;
	}

	bool GLTexture2D::loadFromFile(const std::filesystem::path& filename)
	{
		int width, height, numChannels = 0;
		const std::string adapter = filename.string();
		const char* path = adapter.c_str();

		pixels = stbi_load(path, &width, &height, &numChannels, STBI_default);
		if (!pixels)
		{
			TS_CORE_ERROR("error: in GLTexture2D::loadFromFile, unable to load from path: {}\n", path);
		}

		GLint data_format;
		switch (numChannels) {
		case 4:
			data_format = GL_RGBA;
			break;
		case 3:
			data_format = GL_RGB;
			break;
		default:
			data_format = GL_ZERO;
			TS_CORE_ERROR("error: in GLTexture2D::loadFromFile, unsupported data format with {} channel\n", numChannels);
			break;
		}
		
		if(pixels && data_format != GL_ZERO){
			size.x = static_cast<uint32_t>(width);
			size.y = static_cast<uint32_t>(height);
			this->numChannels = static_cast<uint32_t>(numChannels);

			create(size);
			GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, data_format, width, height, 0, data_format, GL_UNSIGNED_BYTE, pixels));
			TS_CORE_TRACE("load {}-channel texture from {}", numChannels, path);
			//load texture
		}
		else {
			float checkerboard_pixels[] = {
				0.5f, 0.5f, 0.5f,	1.f, 1.f, 1.f,		0.5f, 0.5f, 0.5f,	1.f, 1.f, 1.f,
				1.f, 1.f, 1.f,		0.5f, 0.5f, 0.5f,	1.f, 1.f, 1.f,		0.5f, 0.5f, 0.5f,
				0.5f, 0.5f, 0.5f,	1.f, 1.f, 1.f,		0.5f, 0.5f, 0.5f,	1.f, 1.f, 1.f,
				1.f, 1.f, 1.f,		0.5f, 0.5f, 0.5f,	1.f, 1.f, 1.f,		0.5f, 0.5f, 0.5f
			};

			loadFromMemory(checkerboard_pixels, vec2u{ 4,4 });
			TS_CORE_WARN("unable to load texture: {} - defaulting to {}", path, "default-checkerboard");
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		return true;
	}

	bool GLTexture2D::loadFromMemory(const float* data, vec2u size)
	{
		create(size);

		if (!data) {
			TS_CORE_ERROR("IN GLTexture2D::loadFromMemory, the data is straight up nullptr. T'as foiré big, lol");
			return false;
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_FLOAT, data);
		glBindTexture(GL_TEXTURE_2D, 0);

		return true;
	}


	void GLTexture2D::bind(uint32_t slot)
	{
		TS_CORE_ASSERT(slot < 16, "calme toi avec les texture sample lol, je sais même pas si tous les hardware support autant.");
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, gl_texture_id);
	}
	vec2u GLTexture2D::getSize() const
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

	uint32_t GLTexture2D::getSpecificRenderAPI_ID() const
	{
		return gl_texture_id;
	}
}
