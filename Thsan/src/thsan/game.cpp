#include "pch.h"
#include "thsan/game.h"
#include "thsan/log.h"
#include "thsan/Input/keyboard.h"
#include "SDL.h"
#include "platform/SDL/sdl_window.h"

#include <GL/glew.h>
#include "thsan/graphics/render_target.h"
#include "thsan/graphics/render_command.h"
#include "thsan/graphics/renderer/renderer_2D.h"
#include "thsan/graphics/renderer/renderer_terrain.h"
#include "thsan/graphics/render_context.h"
#include "thsan/graphics/mesh.h"
#include "thsan/graphics/shader.h"
#include "thsan/graphics/framebuffer.h"
#include "thsan/graphics/graphic_api.h"
#include "thsan/state/state_manager.h"
#include "thsan/state/State.h"
#include "thsan/ressource_manager/texture2D_manager.h"
#include <SDL.h>
#include <GL/gl.h>
#include <tsm/math/color.h>

std::chrono::duration<double> frameDuration;

namespace Thsan 
{
	Game::Game():
		isInit{false}
	{
		options.windowOptions.screenWidth = 1280;
		options.windowOptions.screenHeight = 768;

		prevWidth = options.windowOptions.screenWidth;
		prevHeight = options.windowOptions.screenHeight;
	}

	Game::~Game()
	{
		if(renderer2D)
			delete renderer2D;
		if(rendererTerrain)
			delete rendererTerrain;
		if (window)
			delete window;
	}

	StateManager& Game::getStateManager()
	{
		return *stateManager;
	}

	bool Game::init()
	{
		TS_CORE_ASSERT(!isInit, "Game is already initialized, attemp of calling Game::init() more than once is a bad idea, call Game::close() before if you really need to frl.");
		if (!isInit) {
			Keyboard::init();
			getInfo();

			window = new SDLWindow();
			if (!window->create(options.windowOptions.screenWidth, options.windowOptions.screenHeight, "mon jeux"))
				return false;

			RenderContext::init();

			rendererTerrain = RendererTerrain::create();
			renderer2D = Renderer2D::create();

			stateManager = StateManager::create();
			stateManager->init(this);

			isInit = true;

			controlSetting = new ControlSetting();

			debugHUDManager = new DebugHUDManager(window);
		}

		return true;
	}

	void Game::run() 
	{
		this->init();
		onCreate();

		if (stateManager->currentState == "")
		{
			TS_CORE_TRACE("currently no state is set, exiting application.");
			return;
		}

		if(options.renderOptions.enableRendererTerrain)
			rendererTerrain->init(options.renderOptions.terrainOptions, options.windowOptions);
		renderer2D->init();

		State& state = stateManager->getCurrentState();

		state.onStart();

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
			float deltaTime = static_cast<float>(frameTime.count());

			int w = options.windowOptions.screenWidth;
			int h = options.windowOptions.screenHeight;

			if (prevWidth != w || prevHeight != h)
			{
				window->setSize(w, h);
			}

			prevWidth = w;
			prevHeight = h;

			// Add the frame time to the accumulator
			accumulator += frameTime;
			while (accumulator >= frameDuration) {

				update(static_cast<float>(frameDuration.count()));
				draw(static_cast<float>(frameDuration.count()));

				accumulator -= frameDuration;

				if (isDebugUIEnable)
					onUIRender();

				window->swapBuffers();

			}

			std::this_thread::sleep_for(std::chrono::duration<double>(1.0 / targetFrameRate));
		}


		if (stateManager)
			delete stateManager;
	}

	void Game::update(const float& dt)
	{
		std::vector<Event> events = window->pollEvent();

		for (Event e : events) {
			controlSetting->handleInput(e);
		}
		controlSetting->updateInput();

		std::vector<InputAction> inputActions = controlSetting->getInput();
		stateManager->getCurrentState().onInput(dt, inputActions);
		stateManager->getCurrentState().onUpdate(dt);
	}

	void Game::draw(const float& dt)
	{
		if (options.renderOptions.enableRendererTerrain) 
		{
			rendererTerrain->updateTerrainOptions(options.renderOptions.terrainOptions, options.windowOptions);
			rendererTerrain->enableDebug(options.renderOptions.debugRender);

			stateManager->getCurrentState().onDraw(*rendererTerrain, dt);
		}
		stateManager->getCurrentState().onDraw(*renderer2D, dt);

		//imgui specfific
		if (isDebugUIEnable)
		{
			debugHUDManager->render();    // Render debug panels
		}
	}

	bool Game::close()
	{
		debugHUDManager->shutdown();
		if (options.renderOptions.enableRendererTerrain)
			rendererTerrain->close();
		renderer2D->close();
		window->close();
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
		window->enableEventForHUD();
		isDebugUIEnable = true;
	}

	void Game::disableDebugUI()
	{
		isDebugUIEnable = false;
	}

	std::variant<float, glm::vec3, tsm::Color> Game::sampleTerrain(TerrainDataType type, glm::vec2 position)
	{
		return rendererTerrain->sampleTerrain(type, position);
	}

	std::shared_ptr<Thsan::Texture2D> Game::getTexture(const std::string& name)
	{
		return RessourceManager::Texture2DManager::get(name);
	}

	void Game::removeUnused()
	{
		RessourceManager::Texture2DManager::removeUnused();
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