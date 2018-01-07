#pragma once

#include <platforms/platform.h>
#include "string.h"

namespace fd {
namespace utils {

class FileUtils {
public:

	static bool ReadFile(const String& path, void** const data, uint_t* const size);
	static bool ReadFile(const String& path, String* const string);
};

} }