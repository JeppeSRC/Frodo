#include "fileutils.h"
#include <core/log/log.h>

namespace fd {
namespace utils {

using namespace core::log;

bool FileUtils::ReadFile(const String& path, void** const data, uint_t* const size) {
	FILE* file = fopen(*path, "rb");

	if (!file) {
		FD_WARN("[FileUtils] Failed to open file \"%s\"!", *path);
		return false;
	}

	fseek(file, 0, SEEK_SET);
	fseek(file, 0, SEEK_END);
	*size = (uint_t)ftell(file);
	fseek(file, 0, SEEK_SET);

	*data = new unsigned char[*size];

	fread(*data, *size, 1, file);
	fclose(file);

	return true;
}

bool FileUtils::ReadFile(const String& path, String* const string) {
	FILE* file = fopen(*path, "rb");

	if (!file) {
		FD_WARN("[FileUtils] Failed to open file \"%s\"!", *path);
		return false;
	}

	fseek(file, 0, SEEK_SET);
	fseek(file, 0, SEEK_END);
	uint_t size = (uint_t)ftell(file);
	fseek(file, 0, SEEK_SET);

	char* data = new char[size+1];

	fread(data, size, 1, file);
	fclose(file);

	data[size] = 0;

	*string = data;

	return true;
}

}
}