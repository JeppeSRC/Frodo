#include <graphics/pipeline/commandbuffer.h>
#include <core/video/context.h>

namespace fd {
namespace graphics {
namespace pipeline {

using namespace core::video;
using namespace utils;

CommandBuffer::CommandBuffer(VkCommandPool pool, CommandBufferType type) : type(type) {
	VkCommandBufferAllocateInfo info;

	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	info.pNext = nullptr;
	info.level = (VkCommandBufferLevel)type;
	info.commandPool = pool;
	info.commandBufferCount = 1;

	VK(vkAllocateCommandBuffers(Context::GetDevice(), &info, &commandBuffer));
	
}

CommandBuffer::~CommandBuffer() {

}

void CommandBuffer::Begin(CommandBufferUsage usage) {
	VkCommandBufferBeginInfo info;

	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	info.pNext = nullptr;
	info.pInheritanceInfo = nullptr;
	info.flags = (VkCommandBufferUsageFlags)usage;

	VK(vkBeginCommandBuffer(commandBuffer, &info));
}

void CommandBuffer::End() {
	VK(vkEndCommandBuffer(commandBuffer));
}

VkClearColorValue clearColor = { 0.0f, 0.0f, 0.0f, 0.0f };

void CommandBuffer::BeginRenderPass(const RenderPass* const renderPass, uint_t framebufferIndex) {
	VkRenderPassBeginInfo info;

	info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	info.pNext = nullptr;
	info.renderArea.offset.x = 0;
	info.renderArea.offset.y = 0;
	info.renderArea.extent.width = renderPass->GetWidth();
	info.renderArea.extent.height = renderPass->GetHeight();
	info.renderPass = renderPass->GetRenderPass();
	info.framebuffer = renderPass->GetFramebuffer(framebufferIndex);
	info.clearValueCount = 1;
	info.pClearValues = (VkClearValue*)&clearColor;
	
	vkCmdBeginRenderPass(commandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
}

void CommandBuffer::EndRenderPass() {
	vkCmdEndRenderPass(commandBuffer);
}

void CommandBuffer::BindPipeline(const Pipeline* const pipeline) {
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetPipeline());
}

void CommandBuffer::BindPipelineLayout(const PipelineLayout* const pipelineLayout) {
	const List<VkDescriptorSet>& sets = pipelineLayout->GetDescriptorSets();

	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout->GetPipelineLayout(), 0, (uint32)sets.GetSize(), sets.GetData(), 0, 0);
}


}
}
}