#include "StdAfx.h"
#include "Logger.h"
#include <stdarg.h>

Logger::Logger(void)
{
}

Logger::~Logger(void)
{
}

void Logger::setPrefix(std::string prefix)
{
	this->prefix = prefix;
}

void Logger::log(const char* format, ...)
{
	if (!CRAPS_LOGGING_ENABLED)
	{
		return;
	}

    va_list argptr;
    va_start(argptr, format);
	printf(prefix.c_str());
	printf(": ");
    vfprintf(stdout, format, argptr);
	printf("\n");
    va_end(argptr);
}

