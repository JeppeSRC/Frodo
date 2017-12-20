#pragma once

#include "logdevice.h"

#ifdef FD_WINDOWS
#include <Windows.h>
#include <stdio.h>
#else

#endif

namespace fd { 
namespace core {

class LogDeviceConsole : public LogDevice {
private:
#ifdef FD_WINDOWS
	HANDLE consoleHandle;
#endif

public:
	LogDeviceConsole();
	LogDeviceConsole(HANDLE consoleHandle);

	void Log(LogLevel logLevel, const char* const message, va_list list) override;
};

}}