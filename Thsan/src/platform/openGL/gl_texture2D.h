#pragma once
#include "thsan/graphics/texture.h"
#include <glm/ext/vector_uint2.hpp>
#include <unordered_map>
#include <GL/glew.h>

namespace Thsan {
	class GLTexture2D : public Texture2D {
	public:
		// Inherited via Texture2D
		GLTexture2D();
		~GLTexture2D();

		virtual bool isValid() const override;

		virtual bool create(const vec2u& size) override;

		virtual [[nodiscard]] bool create(const glm::uvec2& size, TextureFormat format, const void* data = nullptr, bool mipmapped = false, bool smooth = false) override;

		virtual bool loadFromFile(const std::filesystem::path& filename) override;

		virtual bool loadFromMemory(const float* data, vec2u size) override;

		virtual void bind(uint32_t slot) override;

		virtual vec2u getSize() const override;

		virtual uint32_t getWidth() const override;

		virtual uint32_t getHeight() const override;

		virtual uint32_t getSpecificRenderAPI_ID() const override;
	private:
		vec2u size;
		uint32_t numChannels;
		uint32_t gl_texture_id;
		bool pixelsFlipped;
		bool fboAttachment;
		bool hasMipmap;

		unsigned char* pixels;

	};

	const std::unordered_map<TextureFormat, std::tuple<GLenum, GLenum, GLenum>> FormatMapping = {
	   {TextureFormat::RGBA8, {GL_RGBA8, GL_RGBA, GL_UNSIGNED_INT}},
	   {TextureFormat::R32F, {GL_R32F, GL_RED, GL_FLOAT}},
	   {TextureFormat::RGB16F, {GL_RGB16F, GL_RGB, GL_HALF_FLOAT}},
		// Add more mappings as needed
	};
}