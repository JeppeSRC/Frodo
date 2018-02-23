#pragma once

#include <core/types.h>

#define FD_HEADER_MAKE_SIG(a, b, c, d) (((uint32)d << 24) | ((uint32)c << 16) | ((uint32)b << 8) | a)

#define FD_HEADER_VERSION 0x0001

#define FD_TEXTURE_HEADER_VERSION 0x0001

namespace FD {

enum class TextureChannel : byte {
	R,
	RG,
	RGB,
	RGBA
};

enum class TextureChannelType : byte {
	Uint8
};

enum class FileType : byte {
	Texture,
	Model

};

struct Header {
	uint32 signature;
	uint16 version;
	FD::FileType type;
	uint64 size;
	uint32 compressedSize[2];
};

struct TextureHeader {
	uint16 version;
	FD::TextureChannel pixelLayout;
	FD::TextureChannelType pixelChannelSize;
	uint16 width;
	uint16 height;
};
}