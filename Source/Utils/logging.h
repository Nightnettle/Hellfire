#pragma once

#if DEV_MODE
	#define LOG(x) Utils::Log(x)
#else
	#define LOG(x)
#endif

namespace Utils
{
	void Log(float message);

	void Log(std::string message);
}