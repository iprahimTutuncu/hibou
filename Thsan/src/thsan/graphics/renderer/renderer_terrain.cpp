#include "pch.h"
#include "thsan/graphics/renderer/renderer_terrain.h"
#include "thsan/log.h"
#include "thsan/graphics/render_context.h"
#include "thsan/graphics/render_command.h"
#include "thsan/graphics/framebuffer.h"
#include "thsan/graphics/modifier.h"
#include "thsan/graphics/mesh.h"
#include "thsan/graphics/shader.h"
#include "thsan/graphics/render_states.h"
#include "thsan/graphics/view.h"
#include <thsan/graphics/drawable/sprite_animation.h>
#include <thsan/graphics/modifier/on_terrain_modifier.h>
#include "thsan/graphics/terrain/terrain.h"
#include "thsan/graphics/post_process.h"

namespace Thsan {
	bool RendererTerrainImpl::init(TerrainOptions options, WindowOption windowOptions)
	{
		// ============================
		// RenderContext Pass Setup
		// ============================

		RenderContext::setClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
		RenderContext::setViewport(0, 0, w, h);

		screenView = Thsan::View::create(0, 0, static_cast<float>(w), static_cast<float>(h), 1.f, 1.f, 0.f);
		screenView->setZoom(1.f);

		worldView = Thsan::View::create(0, 0, static_cast<float>(w), static_cast<float>(h), 300.f, 1.f, 0.f); // à voir si 300 est approprié. J'ai oublié c'téait quoi. (depth non?)
		worldView->setZoom(1.f);

		// ============================
		// PostProcress Manager Setup
		// ============================

		postProcessorManager = std::move(Thsan::PostProcessorManager::create());

		// ============================
		// Geometry Buffer Pass Setup
		// ============================

		//todo l'matin: check si la position map est generate sur rednerdoc correctement, faut peut-être mettre external position et calculer selong size du sprite.
		//das le caas du batching faudrait avoir un parametre de height per 4 mesh.

		geometryBufferPassShader = Thsan::Shader::create("media/shader/renderTerrain/geometry_pass.vert", "media/shader/renderTerrain/geometry_pass.frag");
		geometryBufferFramebuffer = Thsan::create_framebuffer(w, h);
		geometryBufferFramebuffer->SetNumColorAttachments(5);
		geometryBufferFramebuffer->attachDepthStencilTarget();
		geometryBufferFramebuffer->attachColorTarget(0, TextureFormat::R32F, glm::vec4(1.0)); //depth
		geometryBufferFramebuffer->attachColorTarget(1, TextureFormat::RGB16F); //normal
		geometryBufferFramebuffer->attachColorTarget(2, TextureFormat::RGBA8); //color
		geometryBufferFramebuffer->attachColorTarget(3, TextureFormat::RGBA8); //specular
		geometryBufferFramebuffer->attachColorTarget(4, TextureFormat::RGBA16F); //position
		
		geometryBufferStates = Thsan::RenderStates::create(RendererType::rendererTerrain);

		geometryBufferStates->setView(worldView);
		geometryBufferStates->setShader(geometryBufferPassShader);

		// ============================
		// Terrain Pass Setup
		// ============================

		terrainPassShader = Thsan::Shader::create("media/shader/renderTerrain/terrain_pass.vert", "media/shader/renderTerrain/terrain_pass.frag");

		terrainFramebuffer = Thsan::create_framebuffer(w, h);
		terrainFramebuffer->SetNumColorAttachments(4);

		terrainFramebuffer->attachColorTarget(0, TextureFormat::RGBA16F); //positionDepth
		terrainFramebuffer->attachColorTarget(1, TextureFormat::RGB16F); //normal

		terrainFramebuffer->attachColorTarget(2, TextureFormat::RGBA8); //color
		terrainFramebuffer->attachColorTarget(3, TextureFormat::RGBA8); //specular

		terrainFramebuffer->initializeDrawBuffers();

		terrainStates = Thsan::RenderStates::create(RendererType::rendererTerrain);

		terrainStates->setView(screenView);
		terrainStates->setShader(terrainPassShader);

		// ============================
		// SSAO Pass Setup
		// ============================

		// ============================
		// SSAO Blur Setup
		// ============================

		// ============================
		// Light Pass Setup
		// ============================

		//to do

		// ============================
		// Post-Process Pass Setup
		// ============================

		postProcessorManager->display();

		postProcessPassShader = Thsan::Shader::create("media/shader/renderTerrain/postprocess_pass.vert", "media/shader/renderTerrain/postprocess_pass.frag");
		postProcessFramebuffer = Thsan::create_framebuffer(w, h);
		postProcessFramebuffer->SetNumColorAttachments(1);
		postProcessFramebuffer->attachColorTarget(0, TextureFormat::RGBA8);

		postProcessStates = Thsan::RenderStates::create(RendererType::rendererTerrain);

		postProcessStates->setView(screenView);
		postProcessStates->setShader(postProcessPassShader);

		// ============================
		// Debug Pass Setup
		// ============================

		debugPassShader = Thsan::Shader::create("media/shader/renderTerrain/debug_pass.vert", "media/shader/renderTerrain/debug_pass.frag");
		debugStates = Thsan::RenderStates::create(RendererType::rendererTerrain);
		debugStates->setView(screenView);
		debugStates->setShader(debugPassShader);


		// ============================
		// screen mesh Pass Setup
		// ============================

		screenQuad = Thsan::Mesh::create(4);

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

		// ============================
		// Create the terrain
		// ============================

		this->updateTerrainOptions(options, windowOptions);

		return true;
	}

	void RendererTerrainImpl::close()
	{
	}

	void RendererTerrainImpl::clear()
	{
		RenderContext::clear();
	}
	void RendererTerrainImpl::setView(std::shared_ptr<View> view)
	{
		geometryBufferStates->setView(view);
	}
	void RendererTerrainImpl::setViewport(int x, int y, int w, int h)
	{
		RenderContext::setViewport(x, y, w, h);
	}
	void RendererTerrainImpl::setDefaultViewport(int x, int y, int w, int h)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}

	void RendererTerrainImpl::display()
	{

		terrainModifier->setView(worldView);
		terrainModifier->update();

		/***************************************************************
		* G-Buffer Pass
		***************************************************************/

		if (!geometryBufferFramebuffer->isComplete())
		{
			TS_CORE_ERROR("Bro tf haha");
		}

		RenderContext::beginPass(geometryBufferFramebuffer);
		RenderContext::clear();
		geometryBufferFramebuffer->clear();

		auto shader = geometryBufferStates->getShader().lock();
		if (shader)
		{
			shader->setFloat("scaleHeight", terrain->getTerrainOptions().scaleHeight);
			shader->setFloat("cameraAngle", terrain->getTerrainOptions().angle);
		}
		geometryBufferStates->setShader(geometryBufferPassShader);

		for (const auto& drawable : drawables) {
			auto draw_command = Thsan::RenderCmd::createRenderDrawableCommand(drawable, geometryBufferStates);
			RenderContext::submit(std::move(draw_command));
		}

		RenderContext::endPass();

		/***************************************************************
		* Terrain Pass
		****************************************************************/

		if (!terrainFramebuffer->isComplete())
		{
			TS_CORE_ERROR("Bro tf haha");
		}

		RenderContext::beginPass(terrainFramebuffer);
		RenderContext::setClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
		RenderContext::clear();
		terrainStates->setShader(terrainPassShader);

		shader = terrainStates->getShader().lock();
		shader->setTexture2D("spritesTextureAlbedo", geometryBufferFramebuffer->getTexture(2));
		shader->setTexture2D("spritesTextureDepth", geometryBufferFramebuffer->getTexture(0));
		shader->setTexture2D("spritesTexturePosition", geometryBufferFramebuffer->getTexture(4));

		auto options = terrain->getTerrainOptions();
		shader->setVec3("cameraPosition", glm::vec3(options.position[0], options.position[1], options.position[2]));
		shader->setTexture2D("terrainTextureDepth", terrain->getRenderedDepthTexture());
		shader->setTexture2D("terrainTextureAlbedo", terrain->getRenderedAlbedoTexture());
		shader->setTexture2D("terrainTexturePosition", terrain->getRenderedPositionTexture());
		shader->setTexture2D("terrainTextureNormal", terrain->getRenderedNormalTexture());


		auto drawScreenTerrainCommand = Thsan::RenderCmd::createRenderMeshCommand(screenQuad, terrainStates);
		RenderContext::submit(std::move(drawScreenTerrainCommand));

		RenderContext::endPass();

		/***************************************************************
		* SSAO Pass
		****************************************************************/

		// TODO

		/***************************************************************
		* SSAO Blur Pass
		****************************************************************/

		// TODO

		/***************************************************************
		* Light Pass
		****************************************************************/

		/*
		// Begin light pass
		RenderContext::beginPass(lightFramebuffer);
		RenderContext::clear();

		for (int i = 0; i < numGBufferTextures; ++i) {
			RenderContext::bindTexture(gBufferTextures[i], i); // Bind texture to texture unit i
		}


		// End light pass
		RenderContext::endPass();
		*/

		// note: post-process doesn't actually have it's own pass. Unless it cause, trouble, I don't see
		// why I shouldn't let the post-procress disable and let the debug go instead. Seem like post-procressing
		// can be done in one pass. This also mean, no bluring effect or any temporal effect (for now)

		/***************************************************************
		* Post-process Pass
		****************************************************************/
		if (debugRender == DebugRender::None)
		{
			RenderContext::beginPass();
			RenderContext::clear();

			postProcessStates->setShader(postProcessPassShader);
			postProcessPassShader->setTexture2D("inputTexture", terrainFramebuffer->getTexture(2));

			auto draw_command = Thsan::RenderCmd::createRenderMeshCommand(screenQuad, postProcessStates);
			RenderContext::submit(std::move(draw_command));

			RenderContext::endPass();
		}

		/***************************************************************
		* Update Screen FBO
		****************************************************************/
		else {
			RenderContext::beginPass();
			RenderContext::clear();

			debugStates->setShader(debugPassShader);
			debugPassShader->setBool("isAlbedo", false);
			debugPassShader->setBool("isNormal", false);
			debugPassShader->setBool("isDepth", false);
			debugPassShader->setBool("isPosition", false);

			if (debugRender == DebugRender::Albedo)
			{
				debugPassShader->setTexture2D("inputTexture", terrainFramebuffer->getTexture(2));
				debugPassShader->setBool("isAlbedo", true);
			}
			else if (debugRender == DebugRender::Normal)
			{
				debugPassShader->setTexture2D("inputTexture", terrainFramebuffer->getTexture(1));
				debugPassShader->setBool("isNormal", true);
			}
			else if (debugRender == DebugRender::Depth)
			{
				debugPassShader->setTexture2D("inputTexture", terrainFramebuffer->getTexture(0));
				debugPassShader->setBool("isDepth", true);
			}
			else if (debugRender == DebugRender::Position)
			{
				debugPassShader->setTexture2D("inputTexture", terrainFramebuffer->getTexture(0));
				debugPassShader->setBool("isPosition", true);
			}
			auto draw_command = Thsan::RenderCmd::createRenderMeshCommand(screenQuad, debugStates);
			RenderContext::submit(std::move(draw_command));

			RenderContext::endPass();

		}

	}

	void RendererTerrainImpl::updateTerrainOptions(TerrainOptions options, WindowOption windowOption)
	{
		if (!terrainModifier)
			terrainModifier = OnTerrainModifier::create(options.terrainSize);

		if (!terrain)
		{
			terrain = Terrain::create();
			terrain->init(options, windowOption);
			//drawables.push_back(terrain);
		}

		terrainModifier->setTerrainOptions(options, windowOption);
		terrain->update(options, windowOption);
	}


	std::variant<float, glm::vec3, tsm::Color> RendererTerrainImpl::sampleTerrain(TerrainDataType dataType, glm::vec2 position)
	{
		switch (dataType)
		{
		case TerrainDataType::PhysicalHeight:
			return terrain->getPhysicalHeight(position);
		case TerrainDataType::Height:
			return terrain->getRenderHeight(position);
		case TerrainDataType::Albedo:
			return terrain->getAlbedo(position);
		case TerrainDataType::Mask:
			return terrain->getCollisionMask(position);
		case TerrainDataType::Normal:
			return terrain->getNormal(position);
		default:
			return 0.0f; // low-key, t'es pas sensé de rendre là
		}
	}

	void RendererTerrainImpl::add(std::shared_ptr<SpriteAnimation> spriteAnimation, std::string name)
	{
		terrainModifier->add(spriteAnimation);
		drawables.push_back(spriteAnimation);

		if (!name.empty())
		{
			if (spriteByTags.find(name) == spriteByTags.end())
			{
				tags.push_back(name);
				spriteByTags[name] = spriteAnimation;
			}
			else
			{
				TS_CORE_WARN("iN RendererTerrainImpl::add, Trying to add a tag that already exist. Try removing the spriteAnimation associated with that tag first");
			}
		}
	}

	void RendererTerrainImpl::remove(std::shared_ptr<Drawable> drawable)
	{
		auto it = std::remove(drawables.begin(), drawables.end(), drawable);
		if (it != drawables.end())
		{
			drawables.erase(it, drawables.end());

			//remove the corresponding sprite from spriteByTags if it exists
			for (auto itTag = spriteByTags.begin(); itTag != spriteByTags.end();)
			{
				if (itTag->second == drawable)
				{
					itTag = spriteByTags.erase(itTag);
					tags.erase(std::remove(tags.begin(), tags.end(), itTag->first), tags.end());
				}
				else
				{
					++itTag;
				}
			}
		}
	}

	std::shared_ptr<SpriteAnimation> RendererTerrainImpl::get(std::string name)
	{
		auto it = spriteByTags.find(name);
		if (it != spriteByTags.end())
		{
			return it->second;
		}
		return nullptr;
	}


	void RendererTerrainImpl::show()
	{
		isShowEnable = true;
	}

	void RendererTerrainImpl::hide()
	{
		isShowEnable = false;
	}

	void RendererTerrainImpl::enableDebug(DebugRender debugRender)
	{
		this->debugRender = debugRender;
	}

	RendererTerrain* RendererTerrain::create()
	{
		return new RendererTerrainImpl();
	}
}