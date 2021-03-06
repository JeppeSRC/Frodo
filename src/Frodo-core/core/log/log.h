#pragma once

#include <core/enums.h>
#include <utils/list.h>

#ifdef FD_WINDOWS
#define FUNCSIG __FUNCSIG__
#elif FD_LINUX
#define FUNCSIG __PRETTY_FUNCTION__
#endif

#define FD_INFO(msg, ...) Log::Info(msg, ##__VA_ARGS__)
#define FD_WARN(msg, ...) Log::Warning(msg, ##__VA_ARGS__)
#define FD_FATAL(msg, ...) Log::Fatal(msg, ##__VA_ARGS__)

#ifdef FD_DEBUG
#define FD_DBG(msg, ...) Log::Debug(msg, ##__VA_ARGS__)
#define FD_ASSERT(x) if (!(x)) { FD_FATAL("Assertion failed \"%s\" FILE=\"%s\" LINE=%u FUNC=\"%s\"", #x, __FILE__, __LINE__, FUNCSIG); int32* abcdefghijklmnopqrstuvwxyz = nullptr; *abcdefghijklmnopqrstuvwxyz = 1;}
#else
#define FD_DBG(msg, ...)
#define FD_ASSERT(x)
#endif

namespace fd { 
namespace core {
namespace log {

class LogDevice;

class Log {
private:
	static utils::List<LogDevice*> devices;

public:
	static void AddDevice(LogDevice* device);
	static void RemoveDevice(LogDevice* device);

	static void Info(const char* const message...);
	static void Debug(const char* const message...);
	static void Warning(const char* const message...);
	static void Fatal(const char* const message...);

	static utils::List<LogDevice*>& GetDevices() { return devices; }
};

}}
}