#pragma once
#include "thsan/core.h"
#include <cstdint>
#include <memory>
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>

//need to re-write an abstraction to remove this
//attachement methode need to be absratcted to remove the GLEnum
//ex: struct targetFormat? struct FrameBufferFormat? 

namespace Thsan {

	class Texture2D;

	class THSAN_API Framebuffer
	{
	public:
		Framebuffer() = default;
		Framebuffer(uint32_t width, uint32_t height);
		virtual ~Framebuffer() = default;

		virtual void bind() = 0;
		virtual void Unbind() = 0;

		glm::uvec2 getSize() const;

		virtual void attachColorTarget(int attachmentIndex, TextureFormat format) = 0;
		virtual void attachDepthStencilTarget() = 0;

		virtual void SetNumColorAttachments(int numAttachments) = 0;

		virtual std::shared_ptr<Texture2D> getTexture(int attachmentIndex) const = 0;
		virtual std::shared_ptr<Texture2D> getDepthTexture() const = 0;

	protected:
		glm::uvec2 size;
	};

	THSAN_API std::shared_ptr<Framebuffer> create_framebuffer(uint32_t width = 0, uint32_t height = 0);

}