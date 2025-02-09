#pragma once
#include "renderer.h"
#include <queue>
#include <memory>
#include <stack>
#include <unordered_set>

namespace Thsan {
	class RenderTarget;
	class Framebuffer;
	class RenderContext;
	class Drawable;
	class Shader;
	class Modifier;
	class Mesh;
	class View;
	class Game;

	class THSAN_API RendererDebugHUD : public Renderer {
	public:
		RendererDebugHUD() = default;
		~RendererDebugHUD() = default;

		virtual bool init() = 0;
		virtual void close() = 0;
		virtual void clear() = 0;
		virtual void setViewport(int x, int y, int w, int h) = 0;
		virtual void setDefaultViewport(int x, int y, int w, int h) = 0;
		virtual void display() = 0;

		virtual void show() = 0;
		virtual void hide() = 0;

	private:
		static RendererDebugHUD* create();

		friend class Game;
	};


	class RendererDebugHUDImpl : public RendererDebugHUD {
	public:
		RendererDebugHUDImpl() = default;
		~RendererDebugHUDImpl() = default;

		bool init() override;
		void close() override;
		void clear() override;
		void setView(std::shared_ptr<View> view) override;
		void setViewport(int x, int y, int w, int h) override;
		void setDefaultViewport(int x, int y, int w, int h) override;
		void display() override;

		void show() override;
		void hide() override;

	private:
		int x{ 0 }, y{ 0 }, w{ 1280 }, h{ 768 };
		bool isShowEnable{ true };
	};

}