
#pragma once

#include <platforms/platform.h>
#include <core/types.h>

namespace fd {
namespace graphics {
namespace buffer {

#ifdef FD_DX

#else

class Buffer {
protected:
	VkBuffer buf;
	VkDeviceMemory deviceMemory;

public:
	Buffer(VkBufferUsageFlags usage, const void* const data, uint64 size);
	~Buffer();

	inline const VkBuffer& GetBuffer() const { return buf; }

};

#endif

}
}
}