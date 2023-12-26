#pragma once
#include "thsan/graphics/texture.h"

namespace Thsan {
	class GLTexture2D : public Texture2D {
	public:
		// Inherited via Texture2D
		GLTexture2D();
		~GLTexture2D();

		virtual bool isValid() const override;

		virtual bool create(const vec2u& size) override;

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
}