#pragma once

#include "pch.h"

#include "platform/SDL/sdl_window.h"
#include "SDL.h"
#include "thsan/log.h"

#include <GL/glew.h>

#include "thsan/graphics/graphic_api.h"
#include "thsan/graphics/framebuffer.h"
#include "thsan/game.h"
#include "platform/openGL/gl_imgui_handler_SDL.h"

#include "thsan/Input/event.h"
#include <set>

#ifdef _WIN32
#pragma comment(linker, "/subsystem:windows")
#endif
#include "imgui/backends/imgui_impl_sdl2.h"

Thsan::SDLWindow::SDLWindow() {

}

Thsan::SDLWindow::~SDLWindow() {
	if (window) {
		close();
	}
}

bool Thsan::SDLWindow::create(const int width, const int height, const char* title)
{
	bool success = true;
	TS_CORE_ASSERT(width > 0 || height > 0,
		"Error: cannot create window context. width and height must both be bigger than 0.");

	this->width = width;
	this->height = height;
	this->title = title;

	setSize(width, height);

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		TS_CORE_ERROR("error initiliazing SDL2: {}", SDL_GetError());
		return false;
	}

	//mettre SDL dans window
	SDL_version version;
	SDL_VERSION(&version);
	TS_CORE_TRACE("SDL {}.{}.{}", version.major, version.minor, version.patch);

	success = setGraphicAPI(get_graphic_API());

	return success;
}

void Thsan::SDLWindow::setTitle(const char* title)
{
	this->title = title;
	SDL_SetWindowTitle(window, title);
}

void Thsan::SDLWindow::setSize(const int width, const int height)
{
	SDL_SetWindowSize(window, width, height);
}

void Thsan::SDLWindow::close()
{
	if (get_graphic_API() == GraphicAPI::vulkan)
		;
	else if (get_graphic_API() == GraphicAPI::openGL)
		destroyContextOpenGL();

	SDL_DestroyWindow(window);

	window = nullptr;
	SDL_Quit();
}

bool Thsan::SDLWindow::isRunning()
{
	return running;
}

void Thsan::SDLWindow::swapBuffers()
{
	SDL_GL_SwapWindow(window);
}

std::vector<Thsan::Event> Thsan::SDLWindow::pollEvent() {
	std::vector<Thsan::Event> events;
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (isEventEnableForHUD)
			ImGui_ImplSDL2_ProcessEvent(&e);

		Event tmp_event;
		if (e.type == SDL_QUIT) {
			running = false;
		}

		if (e.type == SDL_KEYUP) {
			tmp_event.key.code = static_cast<Key>(e.key.keysym.scancode);
			tmp_event.type = Event::KeyReleased;

		}
		else if (e.type == SDL_KEYDOWN) {
			tmp_event.key.code = static_cast<Key>(e.key.keysym.scancode);
			tmp_event.type = Event::KeyPressed;
		}

		if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED) {
			int width = e.window.data1;
			int height = e.window.data2;
			if (resizeCallback) {
				resizeCallback(width, height);
			}
			
		}

		events.push_back(tmp_event);
	}

	return events;
}

void Thsan::SDLWindow::setResizeCallback(std::function<void(int, int)> callback)
{
	resizeCallback = callback;
};

bool Thsan::SDLWindow::createContextOpenGL()
{
	glewExperimental = GL_TRUE;
	glewInit();

	window = SDL_CreateWindow("my application", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!window) {
		TS_CORE_ERROR("Error creating window: {}", SDL_GetError());
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	glContext = SDL_GL_CreateContext(window);

	int major_version, minor_version = -1;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major_version);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor_version);

	TS_CORE_INFO("OpenGL - version {}.{}\n", major_version, minor_version);

	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		TS_CORE_ERROR("error: unable to enable vsynch");
	}

	glewExperimental = GL_TRUE;
	glewInit();

	return true;
}

void Thsan::SDLWindow::destroyContextOpenGL()
{
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
}

std::unique_ptr<Thsan::ImGuiHandler> Thsan::SDLWindow::generateImGuiHandler()
{
	return std::make_unique<GLimGuiHandlerSDL>(window, glContext);
}