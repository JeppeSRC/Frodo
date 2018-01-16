#pragma once

#include "buffer.h"

namespace fd {
namespace graphics {
namespace buffer {

#ifdef FD_DX

#else

class VertexBuffer : public Buffer {
public:
	VertexBuffer(const void* const data, uint64 size);
	~VertexBuffer();

};

#endif

}
}
}