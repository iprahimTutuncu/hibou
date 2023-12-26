#include "pch.h"

#include "window.h"
#include "SDL.h"
#include "log.h"

#include <GL/glew.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.hpp>

#include "thsan/graphics/graphic_api.h"
#include "thsan/graphics/framebuffer.h"
#include "thsan/game.h"

#include "thsan/Input/event.h"

#ifdef _WIN32
#pragma comment(linker, "/subsystem:windows")
#define VK_USE_PLATFORM_WIN32_KHR
#define PLATFORM_SURFACE_EXTENSION_NAME VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#endif

namespace Thsan {

	bool Window::setGraphicAPI(GraphicAPI api)
	{
		bool success = true;

		if (graphicAPI == api)
			return true;


		if (graphicAPI == GraphicAPI::openGL)
			destroyContextOpenGL();
		else if (graphicAPI == GraphicAPI::vulkan)
			destroyContextVulkan();


		graphicAPI = api;

		if (graphicAPI == GraphicAPI::openGL)
			createContextOpenGL();
		else if (graphicAPI == GraphicAPI::vulkan)
			createContextVulkan();
		else if (graphicAPI == GraphicAPI::none) {
			TS_CORE_ERROR("error: In Window::setGraphicAPI,  no graphic API specified");
			success = false;
		}
		else {
			TS_CORE_ERROR("error: In Window::setGraphicAPI, graphic API is unknown");
			success = false;
		}


		return success;
	}
}
