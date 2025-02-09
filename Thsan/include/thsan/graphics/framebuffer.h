#pragma once
#include "thsan/core.h"
#include <cstdint>
#include <memory>
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include "texture.h"

//need to re-write an abstraction to remove this
//attachement methode need to be absratcted to remove the GLEnum
//ex: struct targetFormat? struct FrameBufferFormat? 

namespace Thsan {

	class Texture2D;

	class THSAN_API Framebuffer
	{
	public:
		Framebuffer();
		Framebuffer(uint32_t width, uint32_t height);
		virtual ~Framebuffer() = default;

		virtual void bind() = 0;
		virtual void Unbind() = 0;
		virtual bool isComplete() = 0;

		glm::uvec2 getSize();

		virtual void attachColorTarget(int attachmentIndex, TextureFormat format, glm::vec4 clearColor = glm::vec4(0.0)) = 0;
		virtual void attachDepthStencilTarget() = 0;

		virtual void SetNumColorAttachments(int numAttachments) = 0;
		virtual void initializeDrawBuffers() = 0;

		virtual std::shared_ptr<Texture2D> getTexture(int attachmentIndex) const = 0;
		virtual std::shared_ptr<Texture2D> getDepthStencilTexture() const = 0;

		virtual void clear() = 0;

	protected:
		uint32_t width;
		uint32_t height;
	};

	THSAN_API std::shared_ptr<Framebuffer> create_framebuffer(uint32_t width = 0, uint32_t height = 0);

}