#pragma once

#include "buffer.h"
#include <core/enums.h>

namespace fd {
namespace graphics {
namespace buffer {

class IndexBuffer : public Buffer {
private:
	IndexBufferFormat format;

	uint32 count;

	IndexBuffer(const void* const data, uint64 size, IndexBufferFormat format);
public:
	IndexBuffer(const uint16* const data, uint32 count);
	IndexBuffer(const uint32* const data, uint32 count);
	~IndexBuffer();

	inline uint32 GetCount() const { return count; }

	inline VkIndexType GetFormat() const { return (VkIndexType)format; }
};

}
}
}