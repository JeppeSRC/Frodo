#include <graphics/buffer/vertexbuffer.h>

namespace fd {
namespace graphics {
namespace buffer {


VertexBuffer::VertexBuffer(const void* const data, uint64 size) : Buffer(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, data, size) {
	
}

VertexBuffer::~VertexBuffer() {

}

} } }