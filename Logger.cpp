#include "Logger.h"

void Logger::Log(const char* inFormat, ...)
{
	static char temp[4096]; // not thread safe...
	static wchar_t msg[6000];

	va_list args;
	va_start(args, inFormat);

	_vsnprintf_s(temp, 4096, 4096, inFormat, args);

	size_t len = strlen(temp) + 1;
	mbstowcs_s(&len, msg, temp, 6000);

	OutputDebugString(msg);
	// OutputDebugString("\n");
}

std::string Logger::Sprintf(const char* inFormat, ...)
{
	//not thread safe...
	static char temp[4096];

	va_list args;
	va_start(args, inFormat);

	_vsnprintf_s(temp, 4096, 4096, inFormat, args);
	return std::string(temp);
}