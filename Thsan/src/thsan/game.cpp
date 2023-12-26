#include "pch.h"
#include "Game.h"
#include "log.h"
#include "thsan/Input/keyboard.h"
#include "SDL.h"
#include "platform/SDL/sdl_window.h"
#include "entt.hpp"

#include <GL/glew.h>
#include "thsan/graphics/render_target.h"
#include "thsan/graphics/render_command.h"
#include "thsan/graphics/render_manager.h"
#include "thsan/graphics/mesh.h"
#include "thsan/graphics/shader.h"
#include "thsan/graphics/framebuffer.h"
#include "thsan/graphics/graphic_api.h"
#include "thsan/state/State.h"
#include <SDL.h>
#include <GL/gl.h>

std::chrono::duration<double> frameDuration;

namespace Thsan {

	LogManager Game::logManager;

	Game::Game():
		isInit{false}
	{
	}

	Game::~Game()
	{
		if(renderManager)
			delete renderManager;
		if (window)
			delete window;
	}

	bool Game::init()
	{
		TS_CORE_ASSERT(!isInit, "Game is already initialized, attemp of calling Game::init() more than once is a bad idea, call Game::close() before if you really need to frl.");
		if (!isInit) {
			logManager.init();
			Keyboard::init();
			getInfo();

			window = new SDLWindow();

			if (!window->create(800, 600, "mon jeux"))
				return false;

			renderManager = create_renderManager();
			renderManager->init();
			isInit = true;

			controlSetting = new ControlSetting();
		}

		return true;
	}

	void Game::run() {
		this->init();
		onCreate();
		state->init();

		onUICreate();

		frameDuration = std::chrono::duration<double>(1.0 / targetFrameRate);

		// Initialize variables for tracking time
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto accumulator = std::chrono::duration<double>(0);

		while (window->isRunning()) {

			auto newTime = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> frameTime = newTime - currentTime;
			currentTime = newTime;

			//calculate time in seccond
			float deltaTime = frameTime.count();

			// Add the frame time to the accumulator
			accumulator += frameTime;
			while (accumulator >= frameDuration) {
				update(deltaTime);
				draw(deltaTime);

				accumulator -= frameDuration;

				if (isDebugUIEnable)
					onUIRender();
			}
			//imgui specfific
			window->swapBuffers();

			std::this_thread::sleep_for(std::chrono::duration<double>(1.0 / targetFrameRate));
		}

	}

	void Game::update(const float& dt)
	{
		std::vector<Event> events = window->pollEvent();
		for(Event e: events)
			controlSetting->handleInput(e);
		controlSetting->updateInput();

		std::vector<InputAction> inputActions = controlSetting->getInput();
		state->input(dt, inputActions);
		if (state)
			state->update(dt);
	}

	void Game::draw(const float& dt)
	{
		if (state)
			state->draw(renderManager, dt);
	}

	bool Game::close()
	{
		renderManager->close();
		window->close();
		logManager.close();
		isInit = false;
		return true;
	}

	void Game::getInfo()
	{
		TS_CORE_TRACE("ThsanEngine v{}.{}", 0, 1);
	}

	void Game::add(Key key, InputState inputState, InputAction inputAction)
	{
		TS_CORE_ASSERT(controlSetting != nullptr, "controlSetting in Game::add() class is nullptr, verify that you didn't somehow fuck a private pointer");
		controlSetting->add(key, inputState, inputAction);
	}

	void Game::remove(Key key, InputState inputState, InputAction inputAction)
	{
		TS_CORE_ASSERT(controlSetting != nullptr, "controlSetting in Game::remove() class is nullptr, verify that you didn't somehow fuck a private pointer");
		controlSetting->remove(key, inputState, inputAction);
	}

	void Game::setFPS(double framerate)
	{
		targetFrameRate = framerate;
		frameDuration = std::chrono::duration<double>(1.0 / targetFrameRate);
	}

	double Game::getFPS()
	{
		return targetFrameRate;
	}

	void Game::trace(const std::string& msg)
	{
		TS_CLIENT_TRACE("{}", msg);
	}

	void Game::enableDebugUI()
	{
		isDebugUIEnable = true;
	}

	void Game::disableDebugUI()
	{
		isDebugUIEnable = false;
	}

	void Game::onUICreate()
	{
		TS_CORE_TRACE("default Game::onUICreate() called. No UI window is created.");
	}

	void Game::onUIRender()
	{
		TS_CORE_TRACE("default Game::onUIRender() called. No UI window used.");
	}

}