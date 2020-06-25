#pragma once

class Logger
{
	std::string prefix;

public:
	Logger(void);
	~Logger(void);

	void log(const char* format, ...);
	void setPrefix(std::string prefix);
};
