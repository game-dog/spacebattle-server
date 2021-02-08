#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <Windows.h>
#include <iostream>
#include <cstdarg>

namespace Logger {
	void Log(const char* inFormat, ...);
	std::string Sprintf(const char* inFormat, ...);
}

#define LOG(...) Logger::Log(__VA_ARGS__);

#endif