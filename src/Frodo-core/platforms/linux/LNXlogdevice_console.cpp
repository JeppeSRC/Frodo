#include <core/log/logdevice_console.h>

namespace fd {
namespace core {
namespace log {

LogDeviceConsole::LogDeviceConsole() : LogDevice(LogDeviceType::Console) {

}

void LogDeviceConsole::Log(LogLevel level, const char* message, va_list list) {
    switch(level) {
        case LogLevel::Info:
            printf("\033[1;37mINFO: ");
            break;
        case LogLevel::Debug:
            printf("\033[1;32mDEBUG: ");
            break;
        case LogLevel::Warning:
            printf("\033[1;33mWARNING: ");
            break;
        case LogLevel::Fatal:
            printf("\033[1;31mINFO: ");
            break;
    }

    vprintf(message, list);
    printf("\033[0m\n");
}

}
}
}