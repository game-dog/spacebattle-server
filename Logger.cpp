#include "Logger.h"

void Logger::Log(const char* inFormat, ...)
{
	static char temp[4096]; // not thread safe...

	va_list args;
	va_start(args, inFormat);

	_vsnprintf_s(temp, 4096, 4096, inFormat, args);

	OutputDebugStringA(temp);
	OutputDebugStringA("\n");
}