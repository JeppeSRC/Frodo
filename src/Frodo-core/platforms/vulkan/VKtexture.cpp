#include <graphics/texture/texture.h>
#include <core/video/context.h>
#include <graphics/buffer/buffer.h>

namespace fd {
namespace graphics {
namespace texture {

using namespace core;
using namespace video;
using namespace log;
using namespace buffer;

Texture::Texture() : width(0), height(0) { }

void Texture::CreateImage(uint32 width, uint32 height, VkImageCreateFlagBits flags, VkImageType imageType, VkFormat format, VkImageUsageFlagBits usage, VkMemoryPropertyFlagBits memoryFlags, VkImageLayout initialLayout) {
	this->width = width;
	this->height = height;

	VkImageCreateInfo iinfo;

	VkImageFormatProperties prop;

	bool formatSupported = Context::GetAdapter()->CheckImageFormat(format, imageType, VK_IMAGE_TILING_OPTIMAL, usage, flags, &prop);

	if (!formatSupported) {
		formatSupported = Context::GetAdapter()->CheckImageFormat(format, imageType, VK_IMAGE_TILING_LINEAR, usage, flags, &prop);
		if (formatSupported) {
			FD_WARN("[Texture] Image format supported but only with linear tiling!");
			formatSupported = false;
		} else {
			FD_FATAL("[Texture] Format not supported!");
			return;
		}
	}

	if (width > prop.maxExtent.width || height > prop.maxExtent.height) {
		FD_FATAL("[Texture] To large image %ux%u. Max %ux%u", width, height, prop.maxExtent.width, prop.maxExtent.height);
		return;
	}

	iinfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	iinfo.pNext = nullptr;
	iinfo.flags = flags;
	iinfo.imageType = imageType;
	iinfo.format = format;
	iinfo.extent = { width, height, 1 };
	iinfo.mipLevels = 1;
	iinfo.arrayLayers = flags & VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT ? 6 : 1;
	iinfo.samples = VK_SAMPLE_COUNT_1_BIT;
	iinfo.tiling = formatSupported ? VK_IMAGE_TILING_OPTIMAL : VK_IMAGE_TILING_LINEAR;
	iinfo.usage = usage;
	iinfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	iinfo.queueFamilyIndexCount = 0;
	iinfo.pQueueFamilyIndices = nullptr;
	iinfo.initialLayout = initialLayout;
	
	VK(vkCreateImage(Context::GetDevice(), &iinfo, nullptr, &image));

	VkMemoryRequirements req;
	vkGetImageMemoryRequirements(Context::GetDevice(), image, &req);

	uint32 memIndex = FindMemoryType(Context::GetAdapter()->GetMemoryProperties(), req.memoryTypeBits, memoryFlags);

	VkMemoryAllocateInfo allocInfo;

	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.pNext = nullptr;
	allocInfo.memoryTypeIndex = memIndex;
	allocInfo.allocationSize = req.size;

	VK(vkAllocateMemory(Context::GetDevice(), &allocInfo, nullptr, &imageMemory));

	VK(vkBindImageMemory(Context::GetDevice(), image, imageMemory, 0));

}

}
}
}