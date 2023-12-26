#pragma once

#include "thsan/graphics/Framebuffer.h"

namespace Thsan {

	class Texture2D;

	class GLFramebuffer: public Framebuffer {
	public:
		GLFramebuffer();
		GLFramebuffer(uint32_t width, uint32_t height);
		~GLFramebuffer();
		// Inherited via Framebuffer
		virtual void bind() override;
		virtual void Unbind() override;

		virtual void attachColorTarget(int attachmentIndex, TextureFormat format) override;
		virtual void attachDepthStencilTarget() override;

		virtual void SetNumColorAttachments(int numAttachments) override;

		virtual std::shared_ptr<Texture2D> getTexture(int attachmentIndex) const override;
		virtual std::shared_ptr<Texture2D> getDepthTexture() const override;
	private:
		glm::uvec2 size;
		GLuint fbo{ 0 };
		std::vector<std::shared_ptr<Texture2D>> colorAttachments;
		GLuint depthStencilRenderbuffer;
		std::shared_ptr<Texture2D> depthAttachment;



	};
}