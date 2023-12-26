#include "pch.h"
#include "log_manager.h"
#include "thsan/log.h"

namespace Thsan {

		/*
		spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");

		s_coreLogger = spdlog::stdout_color_mt("THSAN");
		s_coreLogger->set_level(spdlog::level::trace);

		s_clientLogger = spdlog::stdout_color_mt("CLIENT");
		s_clientLogger->set_level(spdlog::level::trace);
		*/

	void LogManager::init()
	{

	}

	void LogManager::close()
	{
	}

}