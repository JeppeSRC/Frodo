#include <graphics/buffer/buffer.h>
#include <core/video/context.h>
#include <core/log/log.h>

namespace fd {
namespace graphics {
namespace buffer {

using namespace core;
using namespace video;
using namespace log;

Buffer::Buffer(VkBufferUsageFlags usage, const void* const data, uint64 size, bool dynamic) : size(size), dynamic(dynamic) {
	VkBufferCreateInfo info;

	info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	info.usage = usage;
	info.size = size;
	info.queueFamilyIndexCount = 0;
	info.pQueueFamilyIndices = nullptr;

	VkBuffer tmpBuf;
	VkDeviceMemory tmpMemory;

	VK(vkCreateBuffer(Context::GetDevice(), &info, nullptr, &tmpBuf));

	VkMemoryRequirements req;

	vkGetBufferMemoryRequirements(Context::GetDevice(), tmpBuf, &req);

	const VkPhysicalDeviceMemoryProperties mem = Context::GetAdapter()->GetMemoryProperties();

	uint32 memIndex = FindMemoryType(mem, req.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	VkMemoryAllocateInfo ainfo;

	ainfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	ainfo.pNext = nullptr;
	ainfo.memoryTypeIndex = memIndex;
	ainfo.allocationSize = req.size;

	VK(vkAllocateMemory(Context::GetDevice(), &ainfo, nullptr, &tmpMemory));

	VK(vkBindBufferMemory(Context::GetDevice(), tmpBuf, tmpMemory, 0));

	void* dank = nullptr;
	VK(vkMapMemory(Context::GetDevice(), tmpMemory, 0, size, 0, &dank));
	if (data) memcpy(dank, data, size);
	vkUnmapMemory(Context::GetDevice(), tmpMemory);

	if (dynamic) {
		buf = tmpBuf;
		deviceMemory = tmpMemory;
	}else {
		info.usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;

		VK(vkCreateBuffer(Context::GetDevice(), &info, nullptr, &buf));

		vkGetBufferMemoryRequirements(Context::GetDevice(), buf, &req);

		memIndex = FindMemoryType(mem, req.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		ainfo.memoryTypeIndex = memIndex;
		ainfo.allocationSize = req.size;

		VK(vkAllocateMemory(Context::GetDevice(), &ainfo, nullptr, &deviceMemory));

		VK(vkBindBufferMemory(Context::GetDevice(), buf, deviceMemory, 0));

		Context::CopyBuffers(&buf, &tmpBuf, &size, 1);

		vkFreeMemory(Context::GetDevice(), tmpMemory, nullptr);
		vkDestroyBuffer(Context::GetDevice(), tmpBuf, nullptr);
	}
}

Buffer::~Buffer() {
	vkDestroyBuffer(Context::GetDevice(), buf, nullptr);
	vkFreeMemory(Context::GetDevice(), deviceMemory, nullptr);
}

void* Buffer::Map(uint64 offset, uint64 size) {
#ifdef FD_DEBUG
	if (!dynamic) {
		FD_FATAL("[Buffer] Buffer not mappable!");
		return nullptr;
	}

	if (offset + size > this->size) {
		FD_FATAL("[Buffer] Buffer mapping out of bounds! Offset(%u) + size(%u) > buffer size(%u)", offset, size, this->size);
		return nullptr;
	}
#endif

	void* mappedData = nullptr;
	VK(vkMapMemory(Context::GetDevice(), deviceMemory, offset, size, 0, &mappedData));
	return mappedData;
}

void Buffer::Unmap() {
	vkUnmapMemory(Context::GetDevice(), deviceMemory);
}

uint32 FindMemoryType(const VkPhysicalDeviceMemoryProperties mem, uint32 type, uint32 prop) {
	for (uint32 i = 0; i < mem.memoryTypeCount; i++) {
		if ((type & (1 << i)) && ((mem.memoryTypes[i].propertyFlags & prop) == prop)) return i;
	}

	return ~0;
}

uint8 GetFormatSize(VkFormat format) {
	if (format >= VK_FORMAT_R8_UNORM && format <= VK_FORMAT_R8_SRGB) {
		return 1;
	} else if (format >= VK_FORMAT_R8G8_UNORM && format <= VK_FORMAT_R8G8_SRGB) {
		return 2;
	} else if (format >= VK_FORMAT_R8G8B8_UNORM && format <= VK_FORMAT_B8G8R8_SRGB) {
		return 3;
	} else if (format >= VK_FORMAT_R8G8B8A8_UNORM && format <= VK_FORMAT_B8G8R8A8_SRGB) {
		return 4;
	} else if (format >= VK_FORMAT_R16_UNORM && format <= VK_FORMAT_R16_SFLOAT) {
		return 2;
	} else if (format >= VK_FORMAT_R16G16_UNORM && format <= VK_FORMAT_R16G16_SFLOAT) {
		return 4;
	} else if (format >= VK_FORMAT_R16G16B16_UNORM && format <= VK_FORMAT_R16G16B16_SFLOAT) {
		return 6;
	} else if (format >= VK_FORMAT_R16G16B16A16_UNORM && format <= VK_FORMAT_R16G16B16A16_SFLOAT) {
		return 8;
	} else if (format >= VK_FORMAT_R32_UINT && format <= VK_FORMAT_R32_SFLOAT) {
		return 4;
	} else if (format >= VK_FORMAT_R32G32_UINT && format <= VK_FORMAT_R32G32_SFLOAT) {
		return 8;
	} else if (format >= VK_FORMAT_R32G32B32_UINT && format <= VK_FORMAT_R32G32B32_SFLOAT) {
		return 12;
	} else if (format >= VK_FORMAT_R32G32B32A32_UINT && format <= VK_FORMAT_R32G32B32A32_SFLOAT) {
		return 16;
	} else if (format >= VK_FORMAT_R64_UINT && format <= VK_FORMAT_R64_SFLOAT) {
		return 8;
	} else if (format >= VK_FORMAT_R64G64_UINT && format <= VK_FORMAT_R64G64_SFLOAT) {
		return 16;
	} else if (format >= VK_FORMAT_R64G64B64_UINT && format <= VK_FORMAT_R64G64B64_SFLOAT) {
		return 24;
	} else if (format >= VK_FORMAT_R64G64B64A64_UINT && format <= VK_FORMAT_R64G64B64A64_SFLOAT) {
		return 32;
	}

	return 0;
}

}
}
}