#pragma once

#include "core.h"
#include "manager/log_manager.h"
#include "thsan/window.h"
#include "thsan/Input/ControlSetting.h"

namespace Thsan {
	class RenderManager;
	class State;
	class ControlSetting;

	class THSAN_API Game
	{
	public:
		Game();
		virtual ~Game();
		virtual void onCreate() = 0;

		void registerCurrState(State* state) {
			this->state = state;
		};

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

		virtual void onUICreate();
		virtual void onUIRender();
	protected:
		bool isInit{false};
		//Managers
		static LogManager logManager;
		Window* window;
		RenderManager* renderManager{ nullptr };
		State* state{nullptr};
	private:
		ControlSetting* controlSetting{ nullptr };
		double targetFrameRate{ 60.0 };

		bool isDebugUIEnable{false};
	};

	Game* create_game();
};