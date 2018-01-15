#pragma once

#include <platforms/platform.h>
#include <core/types.h>

namespace fd {
namespace graphics {
namespace buffer {

#ifdef FD_DX

#else

class VertexBuffer {
private:
	VkBuffer buf;
	VkDeviceMemory deviceMemory;

public:
	VertexBuffer(const void* const data, uint64 size);
	~VertexBuffer();

	inline const VkBuffer& GetBuffer() const { return buf; }

};

#endif

}
}
}