#pragma once

#include <platforms/platform.h>
#include <core/types.h>
#include <utils/string.h>
#include <graphics/buffer/buffer.h>
#include <utils/formats.h>
#include <core/enums.h>

namespace fd {
namespace graphics {
namespace texture {

class Texture {
protected:
	uint32 width;
	uint32 height;

	VkImage image;
	VkDeviceMemory imageMemory;
	VkImageView imageView;

	VkImageCreateInfo iinfo;
	VkImageFormatProperties prop;
	VkMemoryPropertyFlags memoryFlags;

	void CreateImage(uint32 width, uint32 height, VkImageCreateFlags flags, VkImageType imageType, VkFormat format, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryFlags, VkImageLayout initialLayout);
	void RecreateImage(uint32 width, uint32 height);

	Texture(uint32 width = 0, uint32 height = 0, VkImage image = nullptr, VkDeviceMemory memory = nullptr, VkImageView imageView = nullptr);
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
	VkFormat format;

	bool resizable;

	VkImageViewCreateInfo vinfo;
	
	Texture2D(uint32 width, uint32 height, VkFormat format, VkImageUsageFlags usage, VkImageLayout layout);
public:
	Texture2D(const utils::String& filename);

	void Resize(uint32 width, uint32 height);

	inline VkFormat GetFormat() const { return format; }
};

}
}
}