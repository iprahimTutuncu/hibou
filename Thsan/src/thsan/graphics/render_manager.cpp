#include "pch.h"
#include "render_manager.h"
#include "thsan/log.h"
#include "thsan/graphics/render_context.h"
#include "thsan/graphics/render_command.h"
#include "thsan/graphics/framebuffer.h"

namespace Thsan{
	bool RenderManagerImpl::init()
	{
		renderContext = std::make_shared<RenderContext>();
		if (!renderContext->init()) {
			TS_CORE_ERROR("ligne 13, dans RenderManagerImpl::init(), rnderContext s'init pas.");
		}
		renderContext->setClearColor(glm::vec4(0.2f, 0.6f, 0.2f, 1.0f));
		renderContext->setViewport(0, 0, w, h);

		geometryBufferFramebuffer = Thsan::create_framebuffer(w, h);
		geometryBufferFramebuffer->SetNumColorAttachments(5);
		geometryBufferFramebuffer->attachColorTarget(0, TextureFormat::RGB16F); //position
		geometryBufferFramebuffer->attachColorTarget(1, TextureFormat::RGB16F); //normal

		geometryBufferFramebuffer->attachColorTarget(2, TextureFormat::RGBA8); //color
		geometryBufferFramebuffer->attachColorTarget(3, TextureFormat::RGBA8); //specular
		geometryBufferFramebuffer->attachColorTarget(4, TextureFormat::RGB16F); // local normal

		return true; 
	}
	void RenderManagerImpl::close()
	{

	}
	void RenderManagerImpl::clear()
	{
		renderContext->clear();
	}
	void RenderManagerImpl::setViewport(int x, int y, int w, int h)
	{
		renderContext->setViewport(x, y, w, h);
	}
	void RenderManagerImpl::setDefaultViewport(int x, int y, int w, int h)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}

	void RenderManagerImpl::setState(std::shared_ptr<RenderStates2D> states)
	{
		this->states = states;
	}

	void RenderManagerImpl::setState(std::shared_ptr<RenderStates3D> states)
	{
		this->states3D = states;
	}

	void RenderManagerImpl::display()
	{
		renderContext->beginPass();
		renderContext->clear();

		for (const auto& drawable : drawables) {
			auto draw_command = Thsan::RenderCmd::create_renderDrawableCommand(drawable, states);
			renderContext->submit(std::move(draw_command));
		}

		renderContext->endPass();
	}

	void RenderManagerImpl::add(std::shared_ptr<Drawable> drawable)
	{
		drawables.push_back(drawable);
	}

	void RenderManagerImpl::remove(std::shared_ptr<Drawable> drawable)
	{
		std::erase_if(drawables, [drawable](const auto& element) {
			return element == drawable;
		});
	}

	THSAN_API RenderManager* create_renderManager()
	{
		return  new RenderManagerImpl();
	}
}