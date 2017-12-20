#include "log.h"
#include "logdevice.h"

#include <stdarg.h>

namespace fd {
namespace core {

using namespace utils;

List<LogDevice*> Log::devices;

void Log::AddDevice(LogDevice* device) {
	devices.Push_back(device);
}

void Log::RemoveDevice(LogDevice* device) {
	devices.Remove(device);
}

void Log::Info(const char* const message...) {
	va_list list;
	va_start(list, message);

	uint_t size = devices.GetSize();

	for (uint_t i = 0; i < size; i++) {
		devices[i]->Log(LogLevel::Info, message, list);
	}

	va_end(list);
}

void Log::Debug(const char* const message...) {
	va_list list;
	va_start(list, message);

	uint_t size = devices.GetSize();

	for (uint_t i = 0; i < size; i++) {
		devices[i]->Log(LogLevel::Debug, message, list);
	}

	va_end(list);
}

void Log::Warning(const char* const message...) {
	va_list list;
	va_start(list, message);

	uint_t size = devices.GetSize();

	for (uint_t i = 0; i < size; i++) {
		devices[i]->Log(LogLevel::Warning, message, list);
	}

	va_end(list);
}

void Log::Fatal(const char* const message...) {
	va_list list;
	va_start(list, message);

	uint_t size = devices.GetSize();

	for (uint_t i = 0; i < size; i++) {
		devices[i]->Log(LogLevel::Fatal, message, list);
	}

	va_end(list);
}


}}