#pragma once
#include "log.h"

namespace fd { 
namespace core { 

class LogDevice {
protected:
	LogDeviceType type;

	LogDevice(LogDeviceType type);
public:
	virtual ~LogDevice();

	virtual void Log(LogLevel logLevel, const char* const message, va_list list) = 0;

	inline LogDeviceType GetType() const { return type; }
};


}}