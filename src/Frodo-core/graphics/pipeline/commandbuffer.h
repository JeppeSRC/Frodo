#pragma once

#include <core/types.h>
#include <platforms/platform.h>
#include <utils/list.h>
#include <core/enums.h>

namespace fd {
namespace graphics {
namespace pipeline {

class CommandBuffer {
private:
	CommandBufferType type;
	utils::List<VkCommandBuffer> cmdBuffers;

public:
	CommandBuffer(uint32 numBuffers, VkCommandPool pool, CommandBufferType type);
	~CommandBuffer();

};



}
}
}