#pragma once

#ifdef TS_PLATFORM_WINDOWS
	#ifdef TS_BUILD_DLL
		#define THSAN_API __declspec(dllexport)
	#else
		#define THSAN_API __declspec(dllimport)
	#endif
#else
	#error Thsan only support Windows!
#endif //  TS_PLATFORM_WINDOWS
