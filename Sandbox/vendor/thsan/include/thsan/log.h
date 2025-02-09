#pragma once
#include "pch.h"

#define CORE_DEFAULT_LOGGER_NAME "THSAN"
#define CLIENT_DEFAULT_LOGGER_NAME "CLIENT"

#if defined(TS_PLATFORM_WINDOWS)
#define TS_BREAK __debugbreak();
#else
#define TS_BREAK _builtin_trap();
#endif

// Utility function to replace '{}' with the corresponding argument in the string
template<typename T>
void replacePlaceholder(std::string& str, const T& arg) {
    size_t pos = str.find("{}");
    if (pos != std::string::npos) {
        std::ostringstream oss;
        oss << arg;
        str.replace(pos, 2, oss.str());
    }
}

// Custom logging function
template <typename... Args>
void customLog(const char* level, const char* format, Args... args) {
    std::string logMessage(format);
    int dummy[] = { 0, (replacePlaceholder(logMessage, args), 0)... };
    (void)dummy; // To suppress unused variable warning

    std::cout << "[" << level << "] " << logMessage << std::endl;
}

#ifdef TS_CONFIG_DEBUG
#define TS_CORE_TRACE(...)      customLog("CORE_TRACE", __VA_ARGS__)
#define TS_CORE_INFO(...)       customLog("CORE_INFO", __VA_ARGS__)
#define TS_CORE_WARN(...)       customLog("CORE_WARN", __VA_ARGS__)
#define TS_CORE_ERROR(...)      customLog("CORE_ERROR", __VA_ARGS__)
#define TS_CORE_CRITICAL(...)   customLog("CORE_CRITICAL", __VA_ARGS__)

#define TS_CLIENT_TRACE(...)    customLog("CLIENT_TRACE", __VA_ARGS__)
#define TS_CLIENT_INFO(...)     customLog("CLIENT_INFO", __VA_ARGS__)
#define TS_CLIENT_WARN(...)     customLog("CLIENT_WARN", __VA_ARGS__)
#define TS_CLIENT_ERROR(...)    customLog("CLIENT_ERROR", __VA_ARGS__)
#define TS_CLIENT_CRITICAL(...) customLog("CLIENT_CRITICAL", __VA_ARGS__)

#define TS_CORE_ASSERT(x, msg)  if ((x)) {} else {TS_CORE_CRITICAL("ASSERT - {}\n\t{}\n\tin file: {}\n\ton line: {}", x, msg, __FILE__, __LINE__); TS_BREAK};
#define TS_CLIEN_ASSERT(x, msg) if ((x)) {} else {TS_CLIENT_CRITICAL("ASSERT - {}\n\t{}\n\tin file: {}\n\ton line: {}", x, msg, __FILE__, __LINE__); TS_BREAK};
#else
#define TS_CORE_TRACE(...)    (void)0 
#define TS_CORE_WARN(...)     (void)0 
#define TS_CORE_ERROR(...)    (void)0 
#define TS_CORE_INFO(...)     (void)0 
#define TS_CORE_CRITICAL(...) (void)0 

#define TS_CLIENT_TRACE(...)    (void)0
#define TS_CLIENT_INFO(...)     (void)0
#define TS_CLIENT_WARN(...)     (void)0
#define TS_CLIENT_ERROR(...)    (void)0
#define TS_CLIENT_CRITICAL(...) (void)0
#define TS_CORE_ASSERT(x, msg)  if ((x)) {} else {TS_CORE_CRITICAL("ASSERT - {}\n\t{}\n\tin file: {}\n\ton line: {}", x, msg, __FILE__, __LINE__);};
#define TS_CLIEN_ASSERT(x, msg) if ((x)) {} else {TS_CLIENT_CRITICAL("ASSERT - {}\n\t{}\n\tin file: {}\n\ton line: {}", x, msg, __FILE__, __LINE__);};
#endif
