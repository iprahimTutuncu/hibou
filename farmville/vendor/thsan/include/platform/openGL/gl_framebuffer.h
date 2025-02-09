#pragma once

#include "thsan/graphics/Framebuffer.h"
#include <GL/glew.h>

namespace Thsan {

	class Texture2D;

	class GLFramebuffer: public Framebuffer {
	public:
		GLFramebuffer() = default;
		GLFramebuffer(uint32_t width, uint32_t height);
		~GLFramebuffer();
		// Inherited via Framebuffer
		virtual void bind() override;
		virtual void Unbind() override;

		virtual void attachColorTarget(int attachmentIndex, TextureFormat format, glm::vec4 clearColor) override;
		virtual void attachDepthStencilTarget() override;

		virtual void SetNumColorAttachments(int numAttachments) override;
		virtual void initializeDrawBuffers() override;

		bool isComplete() override;

		virtual std::shared_ptr<Texture2D> getTexture(int attachmentIndex) const override;
		virtual std::shared_ptr<Texture2D> getDepthStencilTexture() const override;

		virtual void clear() override;

	private:
		GLuint fbo{ 0 };
		std::vector<std::shared_ptr<Texture2D>> colorAttachments;
		std::vector<glm::vec4> clearColors;
		GLuint depthStencilRenderbuffer{0};
		std::shared_ptr<Texture2D> depthStencilAttachment{ nullptr };
	};
}