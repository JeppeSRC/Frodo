#pragma once

#include "buffer.h"

namespace fd {
namespace graphics {
namespace buffer {


class UniformBuffer : public Buffer {
public:
	UniformBuffer(const void* const data, uint64 size);
	~UniformBuffer();

};

}
}
}