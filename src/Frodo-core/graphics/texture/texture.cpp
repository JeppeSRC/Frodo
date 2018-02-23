#include "texture.h"

#include <utils/lz4/lz4.h>
#include <utils/fileutils.h>
#include <core/log/log.h>

namespace fd {
namespace graphics {
namespace texture {


using namespace FD;

using namespace utils;

using namespace core::log;

void Texture::LoadImageFile(const String& filename, Header* header, TextureHeader* textureHeader, byte** pixels) {
	byte* data = nullptr;
	uint_t dataSize = 0;

	if (!FileUtils::ReadFile(filename, (void**)&data, &dataSize)) {
		*pixels = nullptr;
		return;
	}

	memcpy(header, data, sizeof(Header));
	
	if (header->signature != FD_HEADER_SIGNATURE) {
		Log::Fatal("[Texture] File \"%s\" is not a FD::TextureHeader file!", *filename);
		delete[] data;

		*pixels = nullptr;
	}

	memcpy(textureHeader, data + sizeof(Header), sizeof(TextureHeader));

	byte* tmp = new byte[header->compressedSize[1]];

	LZ4_decompress_safe((const char*)data + sizeof(Header) + sizeof(TextureHeader), (char*)tmp, header->compressedSize[0], header->compressedSize[1]);

	delete[] data;

	*pixels = new byte[header->size - sizeof(TextureHeader)];

	LZ4_decompress_safe((const char*)tmp, (char*)*pixels, header->compressedSize[1], header->size - sizeof(TextureHeader));

	delete[] tmp;
}

}
}
}