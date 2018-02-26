#pragma once

#include <platforms/platform.h>
#include <core/types.h>
#include <utils/string.h>
#include <graphics/buffer/buffer.h>
#include <utils/formats.h>

namespace fd {
namespace graphics {
namespace texture {

#ifdef FD_DX

#else

class Texture {
protected:
	uint32 width;
	uint32 height;

	VkImage image;
	VkDeviceMemory imageMemory;
	VkImageView imageView;

	void CreateImage(uint32 width, uint32 height, VkImageCreateFlags flags, VkImageType imageType, VkFormat format, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryFlags, VkImageLayout initialLayout);

	Texture(uint32 width = 0, uint32 height = 0);
public:
	virtual ~Texture();
	
	inline uint32 GetWidth() const { return width; }
	inline uint32 GetHeight() const { return height; }

	inline VkImage GetImage() const { return image; }
	inline VkImageView GetImageView() const { return imageView; }

public:
	static void LoadImageFile(const utils::String& filename, FD::Header* header, FD::TextureHeader* textureHeader, byte** pixels);
};

class Texture2D : public Texture {
protected:
	
public:
	Texture2D(const utils::String& filename);
};

#endif

}
}
}