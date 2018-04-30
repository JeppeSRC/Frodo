#include <graphics/pipeline/commandbuffer.h>
#include <core/log/log.h>

namespace fd {
namespace graphics {
namespace pipeline {

using namespace core;
using namespace log;

CommandBufferArray::CommandBufferArray(VkCommandPool pool, CommandBufferType type, uint32 count) : commandBuffers(count), count(count) {

	FD_ASSERT(count != 0);

	for (uint32 i = 0; i < count; i++) {
		commandBuffers.Emaplace_back(pool, type);
	}
}

CommandBufferArray::~CommandBufferArray() {

}


void CommandBufferArray::Begin(CommandBufferUsage usage) {
	for (uint32 i = 0; i < count; i++) {
		commandBuffers[i].Begin(usage);
	}
}

void CommandBufferArray::End() {
	for (uint32 i = 0; i < count; i++) {
		commandBuffers[i].End();
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

void CommandBufferArray::BindPipelineLayout(const PipelineLayout* const pipelineLayout) {
	for (uint32 i = 0; i < count; i++) {
		commandBuffers[i].BindPipelineLayout(pipelineLayout);
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

}
}
}