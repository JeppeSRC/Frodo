#include "logdevice_file.h"
#include <ctime>
#include <core/types.h>

namespace fd {
namespace core {
namespace log {

LogDeviceFile::LogDeviceFile(const char* const filename) : LogDevice(LogDeviceType::File) {
	if (filename) {
		file = fopen(filename, "a+");
	} else {
		time_t time = std::time(0);
		struct tm* now = localtime(&time);

		uint16 year = now->tm_year + 1900;
		uint8 month = now->tm_mon + 1;
		uint8 day = now->tm_wday;

		char str[128];
		sprintf(str, "log-%u-%u-%u.txt\0", year, month, day);

		file = fopen(str, "a+");
	}
}

LogDeviceFile::~LogDeviceFile() {
	fclose(file);
}

void LogDeviceFile::Log(LogLevel logLevel, const char* const message, va_list list) {
	time_t time = std::time(0);
	struct tm* now = localtime(&time);

	uint8 hour = now->tm_hour;
	uint8 min = now->tm_min;
	uint8 sec = now->tm_sec;

	switch (logLevel) {
		case LogLevel::Info:
			fprintf(file, "[%u:%u:%u] Info: ", hour, min, sec);
			break;
		case LogLevel::Debug:
			fprintf(file, "[%u:%u:%u] Debug: ", hour, min, sec);
			break;
		case LogLevel::Warning:
			fprintf(file, "[%u:%u:%u] Warning: ", hour, min, sec);
			break;
		case LogLevel::Fatal:
			fprintf(file, "[%u:%u:%u] Fatal: ", hour, min, sec);
			break;

	}

	vfprintf(file, message, list);
	fprintf(file, "\n");
}

}}
}