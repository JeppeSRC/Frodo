#pragma once

#include <core/types.h>
#include <platforms/platform.h>
#include <utils/list.h>
#include <core/enums.h>
#include <graphics/buffer/vertexbuffer.h>
#include <graphics/buffer/indexbuffer.h>
#include <graphics/pipeline/renderpass.h>
#include <graphics/pipeline/pipeline.h>

namespace fd {
namespace graphics {
namespace pipeline {

class CommandBuffer {
private:
	CommandBufferType type;
	VkCommandBuffer commandBuffer;

public:
	CommandBuffer();
	CommandBuffer(VkCommandPool pool, CommandBufferType type);
	~CommandBuffer();

	void Begin(CommandBufferUsage usage);
	void End();

	void ExecuteCommandBuffer(const CommandBuffer* const buffer);

	void BeginRenderPass(const RenderPass* const renderPass, uint_t framebufferIndex = 0);
	void EndRenderPass();

	void BindPipeline(const Pipeline* const pipeline);
	void BindPipelineLayout(const PipelineLayout* const pipelineLayout);
	void Bind(const buffer::VertexBuffer* const vertexBuffer);
	void Bind(const buffer::IndexBuffer* const indexBuffer);

	void DrawIndexed(uint32 count);

	inline CommandBufferType GetType() const { return type; }
	inline const VkCommandBuffer& GetCommandBuffer() const { return commandBuffer; }

};

class CommandBufferArray {
private:
	utils::List<CommandBuffer> commandBuffers;
	uint32 count;
public:
	CommandBufferArray(VkCommandPool pool, CommandBufferType type, uint32 count);
	~CommandBufferArray();

	void Begin(CommandBufferUsage usage);
	void End();

	void ExecuteCommandBuffer(const CommandBuffer* const buffer);

	void BeginRenderPass(const RenderPass* const renderPass);
	void EndRenderPass();

	void BindPipeline(const Pipeline* const pipeline);
	void BindPipelineLayout(const PipelineLayout* const pipelineLayout);
	void Bind(const buffer::VertexBuffer* const vertexBuffer);
	void Bind(const buffer::IndexBuffer* const indexBuffer);

	inline uint32 GetCount() const { return count; }
};
}
}
}