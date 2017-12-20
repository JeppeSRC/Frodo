#pragma once

#include "logdevice.h"

#include <cstdio>

namespace fd {
namespace core {

class LogDeviceFile : public LogDevice {
private:
	FILE* file;

public:
	LogDeviceFile(const char* const filename);
	~LogDeviceFile();

	void Log(LogLevel logLevel, const char* const message, va_list list) override;
};

}}