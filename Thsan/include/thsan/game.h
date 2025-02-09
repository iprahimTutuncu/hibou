#pragma once

#include "core.h"
#include "manager/log_manager.h"
#include "thsan/window.h"
#include "thsan/Input/control_setting.h"
#include "ressource_manager/debugHUD_manager.h"
#include "options.h"
#include <variant>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <thsan/graphics/renderer/renderer_terrain.h>

namespace tsm
{
	class Color;
}

namespace Thsan {
	class Renderer2D;
	class RendererTerrain;
	class State;
	class ControlSetting;
	class StateManager;
	class Texture2D;

	class THSAN_API Game
	{
	public:
		Game();
		virtual ~Game();
		virtual void onCreate() = 0;

		//state management
		StateManager& getStateManager();

		bool init();
		void run();

		void update(const float& dt);
		void draw(const float& dt);

		bool close();
		void getInfo();

		void add(Key key, InputState inputState, InputAction inputAction);
		void remove(Key key, InputState inputState, InputAction inputAction);

		void setFPS(double framerate);
		double getFPS();

		void trace(const std::string& msg);

		void enableDebugUI();
		void disableDebugUI();

		std::variant<float, glm::vec3, tsm::Color> sampleTerrain(TerrainDataType type, glm::vec2 position);

		template<typename PanelDebugType, typename... Args>
		void addDebugPanel(const std::string& name, Args&&... args) {
			debugHUDManager->add<PanelDebugType>(name, std::forward<Args>(args)...);
		}

		std::shared_ptr<Texture2D> getTexture(const std::string& name);
		void removeUnused();

		virtual void onUICreate();
		virtual void onUIRender();

	public:
		Options options;
	protected:
		bool isInit{false};
		//Managers
		DebugHUDManager* debugHUDManager;
		Window* window;
		Renderer2D* renderer2D{ nullptr };
		RendererTerrain* rendererTerrain{ nullptr };

		StateManager* stateManager { nullptr };
	private:
		ControlSetting* controlSetting{ nullptr };
		double targetFrameRate{ 60.0 };

		bool isDebugUIEnable{false};

		int prevWidth;
		int prevHeight;
	};

	Game* create_game();
};