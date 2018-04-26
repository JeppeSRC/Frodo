#include <graphics/pipeline/commandbuffer.h>
#include <core/video/context.h>

namespace fd {
namespace graphics {
namespace pipeline {

using namespace core::video;

CommandBuffer::CommandBuffer(uint32 numBuffers, VkCommandPool pool, CommandBufferType type) : type(type) {
	VkCommandBufferAllocateInfo info;

	cmdBuffers.Resize(numBuffers);

	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	info.pNext = nullptr;
	info.level = (VkCommandBufferLevel)type;
	info.commandPool = pool;
	info.commandBufferCount = numBuffers;

	VK(vkAllocateCommandBuffers(Context::GetDevice(), &info, cmdBuffers.GetData()));
	
}

CommandBuffer::~CommandBuffer() {

}

}
}
}