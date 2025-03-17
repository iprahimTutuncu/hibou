#pragma once
#include "thsan/graphics/texture.h"
#include <glm/ext/vector_uint2.hpp>
#include <unordered_map>
#include <GL/glew.h>

namespace Thsan {

	/*
	
		TODO
			- Dans la m�thode generate, quand y'a R16 j'assume qu'on a pass� un U16 et je converti, c'est kinda retard�, faut que je change ca, j'ai rush
			- la valeur "is16bit", autant dans mon context ca passe, pour de quoi qui se veut r�-utilisable, met un enum pour la paix d'esprit (U8, F16, U16,..)
	
	*/

	class GLTexture2D : public Texture2D {
	public:
		// Inherited via Texture2D
		GLTexture2D();
		~GLTexture2D();

		virtual bool isValid() const override;

		virtual [[nodiscard]] bool generate(const glm::uvec2& size, TextureFormat format, const void* data = nullptr, bool keepRaw = false, bool mipmapped = false, bool smooth = false) override;

		virtual bool loadFromFile(const std::filesystem::path& filename, bool keepRaw, TextureBitDepth bitDepth) override;

		virtual const std::filesystem::path& getFilePath() const override;

		virtual const float* getRawData() const override;

		virtual void bind(uint32_t slot) override;

		virtual glm::uvec2 getSize() const override;

		virtual uint32_t getWidth() const override;

		virtual uint32_t getHeight() const override;

		virtual const int getNumChannels() const override;

		virtual uint32_t getSpecificRenderAPI_ID() const override;

		virtual TextureFormat getTextureFormat() const override;

		virtual void clear(float r = 0.f, float g = 0.f, float b = 0.f, float a = 1.f) override;

		static const std::unordered_map<TextureFormat, std::tuple<GLenum, GLenum, GLenum>> FormatMapping;

	private:
		void initializeTexture(const glm::uvec2& size);

		void generateTextureData(GLenum internalFormat, GLenum glFormat, GLenum glType, int width, int height, const void* data, bool mipmapped, bool smooth, bool keepRaw);

		void generateRawData(GLenum glType, int width, int height, int numChannels, const void* data);


		glm::uvec2 size;
		uint32_t numChannels;
		uint32_t gl_texture_id;
		bool pixelsFlipped;
		bool fboAttachment;
		bool hasMipmap;

		unsigned char* pixels;
		unsigned short* pixels16;
		float* pixels32;
		float* rawData{nullptr};
		TextureFormat textureFormat;
		std::size_t rawSize;
		std::filesystem::path filepath;
	};

}