#include "pch.h"
#include "thsan/log.h"
#include "thsan/options.h"
#include "thsan/graphics/render_context.h"
#include "thsan/graphics/render_command.h"
#include "thsan/graphics/framebuffer.h"
#include "thsan/graphics/modifier.h"
#include "thsan/graphics/mesh.h"
#include "thsan/graphics/shader.h"
#include "thsan/graphics/render_states.h"
#include "thsan/graphics/view.h"
#include "thsan/graphics/renderer/renderer_film.h"
#include "thsan/hud/general_hud/gui.h"
#include "thsan/hud/general_hud/widget.h"


namespace Thsan {
	bool RendererFilmImpl::init(WindowOption windowOptions)
	{
		// ============================
		// RenderContext Pass Setup
		// ============================

		if (!RenderContext::init()) {
			TS_CORE_ERROR("ligne 13, dans RendererFilmImpl::init(), rEnderContext s'init pas.");
			return false;
		}
		RenderContext::setClearColor(glm::vec4(0.8f, 0.4f, 0.2f, 1.0f));
		RenderContext::setViewport(0, 0, w, h);


		// ============================
		// General shader/state Setup
		// ============================
		defaultState = Thsan::RenderStates::create(RendererType::renderer2D);

		defaultShader = Thsan::Shader::create("media/shader/renderFilm/defaultFilm.vert", "media/shader/renderFilm/defaultFilm.frag");
		screenView = Thsan::View::create(0.f, 0.f, static_cast<float>(w), static_cast<float>(h), 1.f, 1.f, 0.f);
		screenView->setZoom(1.f);
		defaultState->setView(screenView);
		defaultState->setShader(defaultShader);

		screenQuad = Thsan::Mesh::create(4);

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

	void RendererFilmImpl::close()
	{
	}

	void RendererFilmImpl::clear()
	{
		RenderContext::clear();
	}
	void RendererFilmImpl::setView(std::shared_ptr<View> view)
	{
		//todo, I don't know lol which renderstate
	}
	void RendererFilmImpl::setViewport(int x, int y, int w, int h)
	{
		RenderContext::setViewport(x, y, w, h);
	}
	void RendererFilmImpl::setDefaultViewport(int x, int y, int w, int h)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}

	void RendererFilmImpl::display()
	{
		if (currentFrame != -1)
			return;

		applyRayTracing(keyframes[currentFrame]);

		/***************************************************************
		* Update Screen FBO
		****************************************************************/

		RenderContext::beginPass();
		RenderContext::clear();

		defaultState->setShader(defaultShader);

		auto draw_command = Thsan::RenderCmd::createRenderMeshCommand(screenQuad, defaultState);
		RenderContext::submit(std::move(draw_command));
		
		RenderContext::endPass();

	}

	void RendererFilmImpl::addLight(LightProperties* light)
	{
	}

	bool RendererFilmImpl::importFrame(std::string frame_path, int index)
	{
		return false;
	}

	bool RendererFilmImpl::exportFrame(std::string frame_path)
	{
		return false;
	}

	void RendererFilmImpl::show()
	{
		isShowEnable = true;
	}

	void RendererFilmImpl::hide()
	{
		isShowEnable = false;
	}

	void RendererFilmImpl::setKeyframe(int frameIndex)
	{
	}

	void RendererFilmImpl::removeKeyframe(int frameIndex)
	{
	}

	void RendererFilmImpl::applyRayTracing(Frame& frame)
	{
		TS_CORE_INFO("APPLYING RAY TRACING TO RESULT");
	}

	RendererFilm* RendererFilm::create()
	{
		return new RendererFilmImpl();
	}
}