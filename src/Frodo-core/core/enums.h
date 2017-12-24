#pragma once

enum class FD_MONITOR_ORIENTATION {
	UNSPECIFIED,
	IDENTITY,
	ROTATE90,
	ROTATE180,
	ROTATE270
};

enum class LogLevel {
	Info,
	Debug,
	Warning,
	Fatal
};

enum class LogDeviceType {
	Console,
	File,
	Custom
};