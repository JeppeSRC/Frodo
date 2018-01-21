#include <graphics/buffer/indexbuffer.h>
#include <core/video/context.h>
#include <core/log/log.h>

namespace fd {
namespace graphics {
namespace buffer {

using namespace core;
using namespace video;

IndexBuffer::IndexBuffer(const void* const data, uint64 size, IndexBufferFormat format) : Buffer(VK_BUFFER_USAGE_INDEX_BUFFER_BIT, data, size), format(format) {
	
}

IndexBuffer::IndexBuffer(const uint16* const data, uint32 count) : IndexBuffer(data, count * sizeof(uint16), IndexBufferFormat::Uint16) { this->count = count; }

IndexBuffer::IndexBuffer(const uint32* const data, uint32 count) : IndexBuffer(data, count * sizeof(uint32), IndexBufferFormat::Uint32) { this->count = count; }

IndexBuffer::~IndexBuffer() {

}

}
}
}