#include "pch.h"
#include "render_manager.h"
#include <GL/glew.h>
#include "thsan/log.h"
#include "thsan/graphics/render_target.h"
#include "thsan/graphics/render_command.h"
#include "thsan/graphics/framebuffer.h"

namespace Thsan{
	bool RenderManagerImpl::init()
	{
		target = create_renderTarget();
		target->setClearColor(0.2f, 0.6f, 0.2f, 1.0f);

		geometryBufferFramebuffer = std::make_shared<Framebuffer>(screen_width, screen_height);
		geometryBufferFramebuffer->SetNumColorAttachments(6);
		geometryBufferFramebuffer->attachColorTarget(0, GL_RGBA16F, GL_RGBA, GL_FLOAT); //position
		geometryBufferFramebuffer->attachColorTarget(1, GL_RGBA16F, GL_RGBA, GL_FLOAT); //normal

		geometryBufferFramebuffer->attachColorTarget(2, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE); //color
		geometryBufferFramebuffer->attachColorTarget(3, GL_RGBA, GL_RGBA, GL_UNSIGNED_INT); //specular
		geometryBufferFramebuffer->attachColorTarget(4, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE); //bject ID
		geometryBufferFramebuffer->attachColorTarget(5, GL_RGBA16F, GL_RGBA, GL_FLOAT); // local normal

		return target->init();
	}
	void RenderManagerImpl::close()
	{
		while (renderCommands.size() > 0)
			renderCommands.pop();
	}
	void RenderManagerImpl::clear()
	{
		TS_CORE_ASSERT(renderCommands.size() == 0, "Error: unflush RenderCommand in queue!");
		target->clear();
	}
	void RenderManagerImpl::setViewport(int x, int y, int w, int h)
	{
		target->setViewport(x, y, w, h);
	}
	void RenderManagerImpl::setDefaultViewport(int x, int y, int w, int h)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}

	std::shared_ptr<Framebuffer> RenderManagerImpl::getActiveFramebuffer()
	{
		return framebuffers.top();
	}
	void RenderManagerImpl::pushFramebuffer(std::shared_ptr<Framebuffer> framebuffer)
	{
		if (framebuffer) {
			framebuffers.push(framebuffer);
			if (framebuffer->setActive(true)) {
				glm::uvec2 size = framebuffer->getSize();
				setViewport(0, 0, size.x, size.y);
			}
		}
		
	}

	void RenderManagerImpl::popFramebuffer()
	{
		TS_CORE_ASSERT(framebuffers.size() > 0, "ASSERT: trying to pop an empty frambuffer stack in RenderManagerImpl::popFramebuffer\n");
		if (framebuffers.size() > 0) {
			if (framebuffers.top()->setActive(false)) {
				framebuffers.pop();
				if (framebuffers.size() > 0) {
					auto fb = framebuffers.top();
					if (fb->setActive(true)) {
						glm::uvec2 size = fb->getSize();
						setViewport(0, 0, size.x, size.y);
					}
				}
				else {
					setViewport(x, y, w, h);
				}
			}
		}
	}
	void RenderManagerImpl::submit(std::unique_ptr<renderCommands::RenderCommand> rc)
	{
		renderCommands.push(std::move(rc));
	}
	void RenderManagerImpl::flush()
	{
		while (renderCommands.size() > 0)
		{
			auto rc = std::move(renderCommands.front());
			renderCommands.pop();

			rc->execute(target, *this);
		}
	}
	THSAN_API RenderManager* create_renderManager()
	{
		return  new RenderManagerImpl();
	}
}