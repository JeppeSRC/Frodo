#pragma once

#include <vector>

#define FD_INFO(msg, ...) Log::Info(msg, __VA_ARGS__)
#define FD_DBG(msg, ...) Log::Debug(msg, __VA_ARGS__)
#define FD_WARN(msg, ...) Log::Warning(msg, __VA_ARGS__)
#define FD_FATAL(msg, ...) Log::Fatal(msg, __VA_ARGS__)

namespace fd { 
namespace core {
		
enum class LogLevel {
	Info,
	Debug,
	Warning,
	Fatal
};

class LogDevice;

class Log {
private:
	static std::vector<LogDevice*> devices;

public:
	static void AddDevice(LogDevice* device);
	static void RemoveDevice(LogDevice* device);

	static void Info(const char* const message...);
	static void Debug(const char* const message...);
	static void Warning(const char* const message...);
	static void Fatal(const char* const message...);

	static std::vector<LogDevice*>& GetDevices() { return devices; }
};

}}