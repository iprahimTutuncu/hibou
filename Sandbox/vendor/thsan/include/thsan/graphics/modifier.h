#pragma once
#include <thsan/core.h>

namespace Thsan {
	class THSAN_API Modifier {
	public:
		Modifier() = default;
		virtual ~Modifier() = default;

		virtual void update() = 0;
	};
}