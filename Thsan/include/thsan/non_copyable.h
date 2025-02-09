#pragma once
#include "thsan/core.h"

namespace Thsan {
	class THSAN_API NonCopyable {
	protected:
		NonCopyable() = default;
		~NonCopyable() = default;
	private:
		NonCopyable(const NonCopyable&) = delete;
		NonCopyable & operator=(const NonCopyable&) = delete;
	};
}