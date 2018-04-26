#pragma once

#include <core/types.h>
#include <platforms/platform.h>
#include <utils/list.h>
#include <core/enums.h>
#include <graphics/buffer/vertexbuffer.h>
#include <graphics/buffer/indexbuffer.h>

namespace fd {
namespace graphics {
namespace pipeline {

class CommandBuffer {
private:
	CommandBufferType type;
	VkCommandBuffer commandBuffer;

public:
	CommandBuffer(VkCommandPool pool, CommandBufferType type);
	~CommandBuffer();

	void Begin(CommandBufferUsage usage);
	void End();

	void BeginRenderPass(const RenderPass* const renderPass, uint_t framebufferIndex = 0);
	void EndRenderPass();

	void BindPipeline(const Pipeline* const pipeline);
	void BindPipelineLayout(const PipelineLayout* const pipelineLayout);
//	void Bind(const buffer::VertexBuffer* const vertexBuffer);
//	void Bind(const buffer::IndexBuffer* const indexBuffer);

};



}
}
}