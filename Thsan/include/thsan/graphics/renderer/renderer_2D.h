#pragma once
#include "renderer.h"
#include <queue>
#include <memory>
#include <stack>
#include <unordered_set>

namespace Thsan::UI
{
	class Gui;
}

namespace Thsan {
	class RenderTarget;
	class Framebuffer;
	class RenderContext;
	class RenderStates;
	class RenderStates3D;
	class Drawable;
	class Shader;
	class Modifier;
	class Mesh;
	class View;
	class Game;

	class THSAN_API Renderer2D: public Renderer {
	public:	
		Renderer2D() = default;
		~Renderer2D() = default;
		
		virtual bool init() = 0;
		virtual void close() = 0;
		virtual void clear() = 0;
		virtual void setView(std::shared_ptr<View> view) = 0;
		virtual void setViewport(int x, int y, int w, int h) = 0;
		virtual void setDefaultViewport(int x, int y, int w, int h) = 0;
		virtual void display() = 0;

		virtual void setGui(std::weak_ptr<UI::Gui> gui) = 0;
		virtual void add(std::shared_ptr<Drawable> drawable) = 0;
		virtual void remove(std::shared_ptr<Drawable> drawable) = 0;

		virtual void show() = 0;
		virtual void hide() = 0;

	private:
		static Renderer2D* create();

		friend class Game;
	};


	class Renderer2DImpl: public Renderer2D {
	public:
		Renderer2DImpl() = default;
		~Renderer2DImpl() = default;

		bool init() override;
		void close() override;
		void clear() override;
		void setView(std::shared_ptr<View> view) override;
		void setViewport(int x, int y, int w, int h) override;
		void setDefaultViewport(int x, int y, int w, int h) override;
		void display() override;

		void setGui(std::weak_ptr<UI::Gui> gui) override;
		void add(std::shared_ptr<Drawable> drawable) override;
		void remove(std::shared_ptr<Drawable> drawable) override;

		void show() override;
		void hide() override;

	private:

		int x{0}, y{ 0 }, w{ 1280 }, h{ 768 };
		bool isShowEnable{ true };

		std::vector<std::shared_ptr<Drawable>> drawables;
		std::weak_ptr<UI::Gui> gui;

		std::shared_ptr<Mesh> screenQuad;
		std::shared_ptr<View> screenView;

		// les shaders
		std::shared_ptr<RenderStates> defaultState;
		std::shared_ptr<RenderStates> guiState;

		std::shared_ptr<Shader> defaultShader = nullptr;
		std::shared_ptr<Shader> guiShader = nullptr;
	};

}