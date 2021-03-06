
#pragma once

#include <platforms/platform.h>
#include <core/types.h>

namespace fd {
namespace graphics {
namespace buffer {

class Buffer {
protected:
	VkBuffer buf;
	VkDeviceMemory deviceMemory;

	uint64 size;

	bool dynamic;

public:
	Buffer(VkBufferUsageFlags usage, const void* const data, uint64 size, bool dynamic = false);
	~Buffer();

	void* Map(uint64 offset, uint64 size);
	void Unmap();

	inline const VkBuffer& GetBuffer() const { return buf; }
	inline const VkDeviceMemory& GetDeviceMemory() const { return deviceMemory; }

};

uint32 FindMemoryType(const VkPhysicalDeviceMemoryProperties mem, uint32 type, uint32 prop);
uint8  GetFormatSize(VkFormat format);

}
}
}