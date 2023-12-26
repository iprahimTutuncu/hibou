#pragma once
#include "thsan/core.h"
#include "thsan/Input/key_code.h"

namespace Thsan {
	class THSAN_API Event {
	public:
		struct KeyEvent {
			Key code;
		};

		union {
			KeyEvent key;
		};

		enum EventType {
			None = 0,
			KeyPressed = 1,
			KeyReleased = 2
		};

		EventType type;
	};
}