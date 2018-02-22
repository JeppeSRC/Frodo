#pragma once

#include <platforms/platform.h>
#include <core/types.h>
#include <utils/string.h>
#include <graphics/buffer/buffer.h>

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

	void CreateImage(uint32 width, uint32 height, VkImageCreateFlagBits flags, VkImageType imageType, VkFormat format, VkImageUsageFlagBits usage, VkMemoryPropertyFlagBits memoryFlags, VkImageLayout initialLayout);

	Texture();
public:
	
	inline uint32 GetWidth() const { return width; }
	inline uint32 GetHeight() const { return height; }

	inline VkImage GetImage() const { return image; }
	inline VkImageView GetImageView() const { return imageView; }
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