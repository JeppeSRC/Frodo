#include "texture.h"

#include <utils/lz4/lz4.h>
#include <utils/fileutils.h>
#include <core/log/log.h>
#include <utils/stb_image.h>

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
		/*Log::Fatal("[Texture] File \"%s\" is not a FD::TextureHeader file!", *filename);
		delete[] data;

		*pixels = nullptr;*/

		int32 x = 0, y = 0, c = 0;

		*pixels = stbi_load_from_memory(data, (int32)dataSize, &x, &y, &c, 4);

		delete[] data;

		if (*pixels == nullptr) {
			Log::Fatal("[Texture] Failed to load file \"%s\"", *filename);
			return;
		}

		textureHeader->width = x;
		textureHeader->height = y;
		textureHeader->pixelChannelSize = TextureChannelType::Uint8;
		header->size = x * y * 4 + sizeof(TextureHeader);
		
		/*switch (c) {
			case 1:
				textureHeader->pixelLayout = TextureChannel::R;
				break;
			case 2:
				textureHeader->pixelLayout = TextureChannel::RG;
				break;
			case 3:
				textureHeader->pixelLayout = TextureChannel::RGB;
				break;
			case 4:
				textureHeader->pixelLayout = TextureChannel::RGBA;
				break;
		}*/

		textureHeader->pixelLayout = TextureChannel::RGBA;

		return;
	}

	memcpy(textureHeader, data + sizeof(Header), sizeof(TextureHeader));

	byte* tmp = (byte*)malloc(header->compressedSize[1]);

	LZ4_decompress_safe((const char*)data + sizeof(Header) + sizeof(TextureHeader), (char*)tmp, header->compressedSize[0], header->compressedSize[1]);

	delete[] data;

	*pixels = (byte*)malloc(header->size - sizeof(TextureHeader));

	LZ4_decompress_safe((const char*)tmp, (char*)*pixels, header->compressedSize[1], header->size - sizeof(TextureHeader));

	free(tmp);
}

}
}
}