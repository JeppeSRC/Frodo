#include <graphics/pipeline/commandbuffer.h>
#include <core/video/context.h>
#include <core/log/log.h>

namespace fd {
namespace graphics {
namespace pipeline {

using namespace core;
using namespace video;
using namespace utils;
using namespace log;

CommandBuffer::CommandBuffer() : commandBuffer(nullptr), type(CommandBufferType::Unknown) {
	Log::Warning("[CommandBuffer] default constructor should not be used");
}

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

void CommandBuffer::Begin(CommandBufferUsage usage, const RenderPass* const renderPass, uint32 framebufferIndex) {
	VkCommandBufferBeginInfo info;

	VkCommandBufferInheritanceInfo hInfo;

	hInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
	hInfo.pNext = nullptr;
	hInfo.subpass = 0; //TODO: handle subpasses
	hInfo.occlusionQueryEnable = false;
	hInfo.queryFlags = 0;
	hInfo.pipelineStatistics = 0;
	hInfo.renderPass = nullptr;
	hInfo.framebuffer = 0;

	info.flags = (VkCommandBufferUsageFlags)usage;

	if (renderPass) {
		hInfo.renderPass = renderPass->GetRenderPass();
		hInfo.framebuffer = renderPass->GetFramebuffer(framebufferIndex);

		info.flags |= VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
	}
	

	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	info.pNext = nullptr;
	info.pInheritanceInfo = &hInfo;

	VK(vkBeginCommandBuffer(commandBuffer, &info));
}

void CommandBuffer::End() {
	VK(vkEndCommandBuffer(commandBuffer));
}

void CommandBuffer::ExecuteCommandBuffer(const CommandBuffer* const buffer) {
	vkCmdExecuteCommands(commandBuffer, 1, &buffer->GetCommandBuffer());
}

void CommandBuffer::ExecuteCommandBuffer(VkCommandBuffer buffer) {
	vkCmdExecuteCommands(commandBuffer, 1, &buffer);
}

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
	
	renderPass->InitializeRenderPass(&info);
	
	vkCmdBeginRenderPass(commandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);

	VkRect2D scissor = { 0, 0, info.renderArea.extent.width , info.renderArea.extent.height };
	VkViewport viewport = { 0.0f, 0.0f, (float)scissor.extent.width, (float)scissor.extent.height, 0.0f, 1.0f };

	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void CommandBuffer::EndRenderPass() {
	vkCmdEndRenderPass(commandBuffer);
}

void CommandBuffer::BindPipeline(const Pipeline* const pipeline) {
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetPipeline());
}

void CommandBuffer::BindDescriptorSet(const PipelineLayout* const layout, uint32 set, const DescriptorSet* const descriptorSet) {
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout->GetPipelineLayout(), set, 1, &descriptorSet->GetDescriptorSet(), 0, nullptr);
}

void CommandBuffer::Bind(const buffer::VertexBuffer* const vertexBuffer) {
	VkDeviceSize offset = 0;
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer->GetBuffer(), &offset);
}

void CommandBuffer::Bind(const buffer::IndexBuffer* const indexBuffer) {
	vkCmdBindIndexBuffer(commandBuffer, indexBuffer->GetBuffer(), 0, indexBuffer->GetFormat());
}

void CommandBuffer::DrawIndexed(uint32 count) {
	vkCmdDrawIndexed(commandBuffer, count, 1, 0, 0, 0);
}

}
}
}