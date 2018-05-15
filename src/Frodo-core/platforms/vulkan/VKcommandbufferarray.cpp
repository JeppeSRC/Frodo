#include <graphics/pipeline/commandbuffer.h>
#include <core/log/log.h>

namespace fd {
namespace graphics {
namespace pipeline {

using namespace core;
using namespace log;

CommandBufferArray::CommandBufferArray(VkCommandPool pool, CommandBufferType type, uint32 count) : commandBuffers(count), count(count) {

	FD_ASSERT(count != 0);

	commandBuffers.Resize(count);

	for (uint32 i = 0; i < count; i++) {
		commandBuffers[i] = CommandBuffer(pool, type);
	}
}

CommandBufferArray::~CommandBufferArray() {
	
}


void CommandBufferArray::Begin(CommandBufferUsage usage, const RenderPass* const renderPass) {
	for (uint32 i = 0; i < count; i++) {
		commandBuffers[i].Begin(usage, renderPass, i);
	}
}

void CommandBufferArray::End() {
	for (uint32 i = 0; i < count; i++) {
		commandBuffers[i].End();
	}
}

void CommandBufferArray::ExecuteCommandBuffer(const CommandBuffer* const buffer) {
	for (uint32 i = 0; i < count; i++) {
		commandBuffers[i].ExecuteCommandBuffer(buffer);
	}
}

void CommandBufferArray::ExecuteCommandBuffer(const CommandBufferArray* const buffer) {
	for (uint32 i = 0; i < count; i++) {
		commandBuffers[i].ExecuteCommandBuffer(buffer->GetCommandBuffer(i));
	}
}

void CommandBufferArray::BeginRenderPass(const RenderPass* const renderPass) {
	for (uint32 i = 0; i < count; i++) {
		commandBuffers[i].BeginRenderPass(renderPass, i);
	}
}

void CommandBufferArray::EndRenderPass() {
	for (uint32 i = 0; i < count; i++) {
		commandBuffers[i].EndRenderPass();
	}
}

void CommandBufferArray::BindPipeline(const Pipeline* const pipeline) {
	for (uint32 i = 0; i < count; i++) {
		commandBuffers[i].BindPipeline(pipeline);
	}
}

void CommandBufferArray::BindDescriptorSet(const PipelineLayout* const layout, uint32 set, const DescriptorSet* const descriptorSet) {
	for (uint32 i = 0; i < count; i++) {
		commandBuffers[i].BindDescriptorSet(layout, set, descriptorSet);
	}
}

void CommandBufferArray::Bind(const buffer::VertexBuffer* const vertexBuffer) {
	for (uint32 i = 0; i < count; i++) {
		commandBuffers[i].Bind(vertexBuffer);
	}
}

void CommandBufferArray::Bind(const buffer::IndexBuffer* const indexBuffer) {
	for (uint32 i = 0; i < count; i++) {
		commandBuffers[i].Bind(indexBuffer);
	}
}

void CommandBufferArray::DrawIndexed(uint32 count) {
	for (uint32 i = 0; i < this->count; i++) {
		commandBuffers[i].DrawIndexed(count);
	}
}

}
}
}