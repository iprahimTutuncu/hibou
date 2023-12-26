#pragma once

#include <vector>
#include <map>
#include "thsan/core.h"
#include "thsan/Input/keyboard.h"
#include "thsan/Input/event.h"

namespace Thsan {
	enum THSAN_API InputState
	{
		isPressed,
		isPressedNoRepeat,
		isReleased,
		isDoubleClick,
		JoystickAxisUp,
		JoystickAxisDown,
		JoystickAxisRight,
		JoystickAxisLeft
	};

	enum THSAN_API InputAction
	{
		up,
		down,
		left,
		right,
		rotateLeft,
		rotateRight,
		start,
		select,
		action
	};

	class ControlSetting {
	public:
		ControlSetting() = default;
		~ControlSetting() = default;
		void add(Key key, InputState inputState, InputAction inputAction);
		void remove(Key key, InputState inputState, InputAction inputAction);

		std::vector<InputAction> getInput();
	private:
		//whoopsy doopsy
		void handleInput(Event event); // create some way to pass event from game.
		void updateInput();

		std::map<std::pair<Key, InputState>, std::vector<InputAction>> inputMap;
		std::vector<InputAction> values;
		//std::map<sf::Keyboard::Key, bool> keyReleasedCheker;
		std::map<std::pair<Key, InputState>, bool> inputActive;

		friend class Game;
	};
}