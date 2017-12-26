#include <core/log/logdevice_console.h>

#define FD_LOG_COLOR_INFO 0b00001111
#define FD_LOG_COLOR_DEBUG 0b00001010
#define FD_LOG_COLOR_WARNING 0b00001110
#define FD_LOG_COLOR_FATAL 0b00001100

namespace fd {
namespace core {

LogDeviceConsole::LogDeviceConsole() : LogDevice(LogDeviceType::Console) {
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
}

LogDeviceConsole::LogDeviceConsole(HANDLE consoleHandle) : LogDevice(LogDeviceType::Console), consoleHandle(consoleHandle) {

}

void LogDeviceConsole::Log(LogLevel logLevel, const char* const message, va_list list) {

	CONSOLE_SCREEN_BUFFER_INFO info;

	GetConsoleScreenBufferInfo(consoleHandle, &info);

	switch (logLevel) {
		case LogLevel::Info: 
			SetConsoleTextAttribute(consoleHandle, FD_LOG_COLOR_INFO);
			printf("INFO: ");
			vprintf(message, list);
			printf("\n");
			break;
		case LogLevel::Debug:
			SetConsoleTextAttribute(consoleHandle, FD_LOG_COLOR_DEBUG);
			printf("DEBUG: ");
			vprintf(message, list);
			printf("\n");
			break;
		case LogLevel::Warning:
			SetConsoleTextAttribute(consoleHandle, FD_LOG_COLOR_WARNING);
			printf("WARNING: ");
			vprintf(message, list);
			printf("\n");
			break;
		case LogLevel::Fatal:
			SetConsoleTextAttribute(consoleHandle, FD_LOG_COLOR_FATAL);
			printf("FATAL: ");
			vprintf(message, list);
			printf("\n");
			break;
	}

	SetConsoleTextAttribute(consoleHandle, info.wAttributes);

}

}}