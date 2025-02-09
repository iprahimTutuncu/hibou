#include "pch.h"
#include "thsan/log.h"
#include "thsan/graphics/render_context.h"
#include "thsan/graphics/render_command.h"
#include "thsan/graphics/framebuffer.h"
#include "thsan/graphics/modifier.h"
#include "thsan/graphics/mesh.h"
#include "thsan/graphics/shader.h"
#include "thsan/graphics/render_states.h"
#include "thsan/graphics/view.h"
#include "thsan/graphics/renderer/renderer_2D.h"
#include "thsan/hud/general_hud/gui.h"
#include "thsan/hud/general_hud/widget.h"


namespace Thsan{
	bool Renderer2DImpl::init()
	{
		// ============================
		// RenderContext Pass Setup
		// ============================

		if (!RenderContext::init()) {
			TS_CORE_ERROR("ligne 13, dans Renderer2DImpl::init(), rEnderContext s'init pas.");
			return false;
		}
		RenderContext::setClearColor(glm::vec4(0.8f, 0.4f, 0.2f, 1.0f));
		RenderContext::setViewport(0, 0, w, h);


		// ============================
		// General shader/state Setup
		// ============================
		defaultState = Thsan::RenderStates::create(RendererType::renderer2D);

		defaultShader = Thsan::Shader::create("media/shader/render2D/default2D.vert", "media/shader/render2D/default2D.frag");
		screenView = Thsan::View::create(0.f, 0.f, static_cast<float>(w), static_cast<float>(h), 1.f, 1.f, 0.f);
		screenView->setZoom(1.f);
		defaultState->setView(screenView);
		defaultState->setShader(defaultShader);

		screenQuad = Thsan::Mesh::create(4);

		// ============================
		// GUI-specific shader/state Setup
		// ============================		
		guiShader = Thsan::Shader::create("media/shader/render2D/gui.vert", "media/shader/render2D/gui.frag");
		guiState = Thsan::RenderStates::create(RendererType::renderer2D);
		guiState->setView(screenView);
		guiState->setShader(guiShader);

		// ============================
		// screen mesh Pass Setup
		// ============================

		(*screenQuad)[0].position = glm::vec3{ 0.f, 0.f, 0.f };
		(*screenQuad)[0].texCoord = glm::vec2{ 0.f, 0.f };
		  
		(*screenQuad)[1].position = glm::vec3{ w, 0.f, 0.f };
		(*screenQuad)[1].texCoord = glm::vec2{ 1.0f, 0.f };
		  
		(*screenQuad)[2].position = glm::vec3{ w, h, 0.f };
		(*screenQuad)[2].texCoord = glm::vec2{ 1.0f, 1.f };
		  
		(*screenQuad)[3].position = glm::vec3{ 0.f, h, 0.f };
		(*screenQuad)[3].texCoord = glm::vec2{ 0.f, 1.f };

		screenQuad->setIndices({ 0, 1, 2, 0, 2, 3 });

		screenQuad->setPrimitiveType(Thsan::PrimitiveType::Triangles);
		screenQuad->generate();
		return true; 
	}
	
	void Renderer2DImpl::close()
	{
	}

	void Renderer2DImpl::clear()
	{
		RenderContext::clear();
	}
	void Renderer2DImpl::setView(std::shared_ptr<View> view)
	{
		//todo, I don't know lol which renderstate
	}
	void Renderer2DImpl::setViewport(int x, int y, int w, int h)
	{
		RenderContext::setViewport(x, y, w, h);
	}
	void Renderer2DImpl::setDefaultViewport(int x, int y, int w, int h)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}

	void Renderer2DImpl::display()
	{
		/***************************************************************
		* Update Screen FBO
		****************************************************************/
		
		RenderContext::beginPass();

		for (const auto& drawable : drawables) {
			auto draw_command = Thsan::RenderCmd::createRenderDrawableCommand(drawable, defaultState);
			RenderContext::submit(std::move(draw_command));
		}

		if (auto guiPtr = gui.lock())
		{
			std::vector<std::shared_ptr<UI::Widget>> children = guiPtr->getWidgets();
			for (const auto& child : children) 
			{
				auto drawCommand = Thsan::RenderCmd::createRenderDrawableCommand(child, guiState);
				RenderContext::submit(std::move(drawCommand));
			}
		}

		RenderContext::endPass();
		
	}

	void Renderer2DImpl::setGui(std::weak_ptr<UI::Gui> gui)
	{
		this->gui = gui;
	}

	void Renderer2DImpl::add(std::shared_ptr<Drawable> drawable)
	{
		drawables.push_back(drawable);
	}

	void Renderer2DImpl::remove(std::shared_ptr<Drawable> drawable)
	{
		std::erase_if(drawables, [drawable](const auto& element) {
			return element == drawable;
		});
	}


	void Renderer2DImpl::show()
	{
		isShowEnable = true;
	}

	void Renderer2DImpl::hide()
	{
		isShowEnable = false;
	}

	Renderer2D* Renderer2D::create()
	{
		return new Renderer2DImpl();
	}
}