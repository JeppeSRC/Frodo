#pragma once

#include "logdevice.h"
#include <stdio.h>

#include <platforms/platform.h>

namespace fd { 
namespace core {
namespace log {

class LogDeviceConsole : public LogDevice {
private:
#ifdef FD_WINDOWS
	HANDLE consoleHandle;
#endif

public:
	LogDeviceConsole();
	#ifdef FD_WINDOWS
	LogDeviceConsole(HANDLE consoleHandle);
	#endif

	void Log(LogLevel logLevel, const char* const message, va_list list) override;
};

}}
}