#include <graphics/buffer/uniformbuffer.h>

namespace fd {
namespace graphics {
namespace buffer {


UniformBuffer::UniformBuffer(const void* const data, uint64 size) : Buffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, data, size, true) {

}

UniformBuffer::~UniformBuffer() {

}

}
}
}