#pragma once

#include <../Frodo-core/core/types.h>

#define FD_CURRENT_FORMAT_VERSION 0x0001

namespace fd {
namespace format {

enum class FileType : uint8 {
	Unknown,
	Binary,
	Texture,
	Model
};

struct FDFHeader {
	uint16 version;
	FileType type;
	uint64 size;
};

struct FDFTextureHeader {
	uint8 channelCount;
	uint16 width;
	uint16 height;
};



}}