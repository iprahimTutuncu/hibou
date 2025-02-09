#pragma once
#include <thsan/core.h>
#include <memory>

namespace Thsan{
	class RenderStates;
	class Drawable;
	class Modifier;
	class View;

	enum class DebugRender
	{
		None,
		Position,
		Depth,
		Normal,
		Albedo
	};

	class THSAN_API Renderer {
	public:
		Renderer() = default;
		~Renderer() = default;

		virtual void close() = 0;
		virtual void clear() = 0;
		virtual void setView(std::shared_ptr<View> view) = 0;
		virtual void setViewport(int x, int y, int w, int h) = 0;
		virtual void setDefaultViewport(int x, int y, int w, int h) = 0;
		virtual void display() = 0;

		virtual void show() = 0;
		virtual void hide() = 0;

		virtual void enableDebug(DebugRender debugRender) {};
	};
}