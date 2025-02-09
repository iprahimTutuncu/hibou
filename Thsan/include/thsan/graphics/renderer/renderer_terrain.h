#pragma once

#include "renderer.h"
#include <queue>
#include <memory>
#include <stack>
#include <unordered_set>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <variant>
#include <unordered_map>

namespace tsm
{
	class Color;
}

namespace Thsan {
	class RenderTarget;
	class Framebuffer;
	class RenderStates;
	class Drawable;
	class Shader;
	class Modifier;
	class Mesh;
	class View;
	class game;
	class SpriteAnimation;
	class Terrain;
	class OnTerrainModifier;
	class PostProcessorManager;
	struct TerrainOptions;
	struct WindowOption;

	enum class THSAN_API TerrainDataType 
	{
		PhysicalHeight,
		Height,
		Albedo,
		Mask,
		Normal
	};

	class THSAN_API RendererTerrain: public Renderer {

	public:
		RendererTerrain() = default;
		~RendererTerrain() = default;

		virtual bool init(TerrainOptions options, WindowOption windowOptions) = 0;
		virtual void close() = 0;
		virtual void clear() = 0;
		virtual void setView(std::shared_ptr<View> view) = 0;
		virtual void setViewport(int x, int y, int w, int h) = 0;
		virtual void setDefaultViewport(int x, int y, int w, int h) = 0;
		virtual void display() = 0;

		virtual void updateTerrainOptions(TerrainOptions desc, WindowOption windowOption) = 0;

		virtual std::variant<float, glm::vec3, tsm::Color> sampleTerrain(TerrainDataType dataType, glm::vec2 position) = 0;

		virtual void add(std::shared_ptr<SpriteAnimation> spriteAnimation, std::string name = "") = 0;
		virtual void remove(std::shared_ptr<Drawable> drawable) = 0;
		virtual std::shared_ptr<SpriteAnimation> get(std::string name) = 0;
		virtual const std::vector<std::string>& getAllTags() const = 0;

		virtual void show() = 0;
		virtual void hide() = 0;

	private:
		static RendererTerrain* create();

		friend class Game;
	};


	class RendererTerrainImpl : public RendererTerrain {
	public:
		RendererTerrainImpl() = default;
		~RendererTerrainImpl() = default;

		bool init(TerrainOptions options, WindowOption windowOptions) override;
		void close() override;
		void clear() override;
		void setView(std::shared_ptr<View> view) override;
		void setViewport(int x, int y, int w, int h) override;
		void setDefaultViewport(int x, int y, int w, int h) override;
		void display() override;
		void updateTerrainOptions(TerrainOptions desc, WindowOption windowOption) override;

		std::variant<float, glm::vec3, tsm::Color> sampleTerrain(TerrainDataType dataType, glm::vec2 position) override;

		void add(std::shared_ptr<SpriteAnimation> spriteAnimation, std::string name = "") override;
		void remove(std::shared_ptr<Drawable> drawable)  override;
		std::shared_ptr<SpriteAnimation> get(std::string name) override;
		const std::vector<std::string>& getAllTags() const override { return tags; }

		void show() override;
		void hide() override;

		virtual void enableDebug(DebugRender debugRender) override;

	private:

		int x{ 0 }, y{ 0 }, w{ 1280 }, h{ 768 };
		bool isShowEnable{ true };

		DebugRender debugRender{ DebugRender::None };

		std::shared_ptr<OnTerrainModifier> terrainModifier;

		std::shared_ptr<Terrain> terrain;
		std::vector<std::shared_ptr<Drawable>> drawables;

		std::shared_ptr<Mesh> screenQuad;
		std::shared_ptr<View> screenView;
		std::shared_ptr<View> worldView;

		/*
			refacto, have a class called RenderPass that encapsualte these variable that
			follow. Vulkan contains VkRenderPass, might be nice to abstract that concept.
		*/

		//current framebuffer for the passes
		std::shared_ptr<Framebuffer> geometryBufferFramebuffer;
		std::shared_ptr<Framebuffer> postProcessFramebuffer;
		std::shared_ptr<Framebuffer> terrainFramebuffer;

		//current state for the passes
		std::shared_ptr<RenderStates> geometryBufferStates = nullptr;
		std::shared_ptr<RenderStates> postProcessStates = nullptr;
		std::shared_ptr<RenderStates> debugStates = nullptr;
		std::shared_ptr<RenderStates> terrainStates = nullptr;

		//current shader for the passes
		std::shared_ptr<Shader> geometryBufferPassShader = nullptr;
		std::shared_ptr<Shader> terrainPassShader = nullptr;
		std::shared_ptr<Shader> lightPassShader = nullptr;
		std::shared_ptr<Shader> postProcessPassShader = nullptr;
		std::shared_ptr<Shader> debugPassShader = nullptr;
		std::shared_ptr<Shader> ssaoPassShader = nullptr;
		std::shared_ptr<Shader> ssaoBlurPassShader = nullptr;
		std::shared_ptr<Shader> shadowPassShader = nullptr;

		std::unique_ptr<PostProcessorManager> postProcessorManager = nullptr;

		std::vector<std::string> tags;
		std::unordered_map<std::string, std::shared_ptr<SpriteAnimation>> spriteByTags;

	};

}