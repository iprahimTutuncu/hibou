#pragma once
#include "core.h"
#include "thsan/graphics/graphic_api.h"

namespace  Thsan {

	class Framebuffer;
	class Game;
	class Event;
	class ImGuiHandler;

	class THSAN_API Window {
	public:
		Window() = default;
		virtual ~Window() = default;

		virtual bool create(const int width, const int height, const char* title) = 0;
		virtual void setTitle(const char* title) = 0;
		virtual void setSize(const int width, const int height) = 0;
		virtual void close() = 0;
		virtual bool isRunning() = 0;
		virtual void swapBuffers() = 0;
		virtual void enableEventForHUD() { isEventEnableForHUD = true; }
		virtual void disableEventForHUD() { isEventEnableForHUD = false; }
		virtual std::vector<Event> pollEvent() = 0;
		virtual void setResizeCallback(std::function<void(int, int)> callback) = 0;
		virtual std::unique_ptr<ImGuiHandler> generateImGuiHandler() = 0;

		bool setGraphicAPI(GraphicAPI api);

	protected:
		virtual bool createContextOpenGL() = 0;
		virtual void destroyContextOpenGL() = 0;

		int width{ 0 };
		int height{ 0 };
		bool running{ true };
		bool isEventEnableForHUD{ false };
		const char* title{ nullptr };
		GraphicAPI graphicAPI{ GraphicAPI::none };
	};
}
