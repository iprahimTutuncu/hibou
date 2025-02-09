#pragma once
#include "pch.h"
#include "thsan/core.h"

namespace Thsan {
	class THSAN_API LogManager 
	{

	public:
		LogManager() = default;
		~LogManager() = default;

		void init();
		void close();
	private:

	};
};

