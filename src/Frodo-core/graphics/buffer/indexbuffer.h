#pragma once

#include <platforms/platform.h>
#include <core/types.h>
#include <core/enums.h>

namespace fd {
namespace graphics {
namespace buffer {

#ifdef FD_DX

#else

class IndexBuffer {
private:
	VkBuffer buf;
	VkDeviceMemory deviceMemory;

	IndexBufferFormat format;

	uint32 count;

	IndexBuffer(const void* const data, uint64 size, IndexBufferFormat format);
public:
	IndexBuffer(const uint16* const data, uint32 count);
	IndexBuffer(const uint32* const data, uint32 count);
	~IndexBuffer();

	inline VkBuffer GetBuffer() const { return buf; }
	inline IndexBufferFormat GetFormat() const { return format; }
};

#endif

}
}
}