#pragma once

#include "buffer.h"

namespace fd {
namespace graphics {
namespace buffer {


class VertexBuffer : public Buffer {
public:
	VertexBuffer(const void* const data, uint64 size);
	~VertexBuffer();

};


}
}
}