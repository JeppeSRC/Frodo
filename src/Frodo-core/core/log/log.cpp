#include "log.h"
#include "logdevice.h"

#include <core/types.h>
#include <stdarg.h>


namespace fd {
namespace core {

std::vector<LogDevice*> Log::devices;

void Log::AddDevice(LogDevice* device) {
	devices.push_back(device);
}

void Log::RemoveDevice(LogDevice* device) {
	for (uint_t i = 0; i < devices.size(); i++) {
		if (devices[i] == device) {
			devices.erase(devices.begin() + i);
		}
	}
}

void Log::Info(const char* const message...) {
	va_list list;
	va_start(list, message);

	uint_t size = devices.size();

	for (uint_t i = 0; i < size; i++) {
		devices[i]->Log(LogLevel::Info, message, list);
	}

	va_end(list);
}

void Log::Debug(const char* const message...) {
	va_list list;
	va_start(list, message);

	uint_t size = devices.size();

	for (uint_t i = 0; i < size; i++) {
		devices[i]->Log(LogLevel::Debug, message, list);
	}

	va_end(list);
}

void Log::Warning(const char* const message...) {
	va_list list;
	va_start(list, message);

	uint_t size = devices.size();

	for (uint_t i = 0; i < size; i++) {
		devices[i]->Log(LogLevel::Warning, message, list);
	}

	va_end(list);
}

void Log::Fatal(const char* const message...) {
	va_list list;
	va_start(list, message);

	uint_t size = devices.size();

	for (uint_t i = 0; i < size; i++) {
		devices[i]->Log(LogLevel::Fatal, message, list);
	}

	va_end(list);
}


}}