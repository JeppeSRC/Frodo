#pragma once
#include <graphics/buffer/bufferlayout.h>
#include <graphics/buffer/uniformbuffer.h>
#include <graphics/shader/shader.h>
#include <graphics/texture/texture.h>
#include <graphics/texture/sampler.h>
#include <core/types.h>
#include <core/enums.h>

namespace fd {
namespace graphics {
namespace pipeline {

struct ViewportInfo {
	float32 x;
	float32 y;
	float32 width;
	float32 height;
	float32 minDepth;
	float32 maxDepth;
};

struct ScissorInfo {
	uint32 x;
	uint32 y;
	uint32 width;
	uint32 height;
};

struct DepthStencilOp {
	StencilOp stencilPass;
	StencilOp stencilFail;
	StencilOp depthFail;
	ComparisonFunc compare;
};

struct DepthStencilInfo {
	bool depthEnable;
	bool depthWrite;
	ComparisonFunc depthFunc;
	bool stencilEnable;
	DepthStencilOp frontFace;
	DepthStencilOp backFace;
};

typedef uint8 ColorWriteMaskFlags;

struct BlendInfo {
	bool blendEnable;
	BlendFactor srcBlend;
	BlendFactor destBlend;
	BlendOp blendOp;
	BlendFactor alpahSrcBlend;
	BlendFactor alphaDestBlend;
	BlendOp alphaBlendOp;
	ColorWriteMaskFlags colorWriteMask;
};

typedef uint32 ShaderAccessFlags;

struct PipelineLayoutElement {
	BufferType type;
	uint64 size;
	uint32 count;
	ShaderAccessFlags shaderAccess;
};

struct PipelineLayout {
	uint32 numElements;
	PipelineLayoutElement* elements;
};

struct PipelineInfo {
	uint32 numViewports;
	ViewportInfo* viewports;

	uint32 numScissors;
	ScissorInfo* scissors;

	PrimitiveTopology topology;
	PolygonMode polygonMode;

	shader::Shader* shader;

	uint32 numInputLayouts;
	buffer::BufferLayout* shaderInputLayouts;

	PipelineLayout pipelineLayout;

	DepthStencilInfo depthStencilInfo;

	uint8 numBlends;
	BlendInfo* blends;

	CullMode cullMode;
	FrontFace frotFace;
};

#ifdef FD_DX

class Pipeline {
private:
};

#else 

class Pipeline {
private:
	struct DescriptorElement {
		DescriptorElement(ShaderAccessFlags accessFlags, BufferType type, uint32 count) : shaderAccess(accessFlags), type(type), count(count) { }
		ShaderAccessFlags shaderAccess;
		BufferType type;
		uint32 count;
	};

	struct UniformElement : public DescriptorElement {
		UniformElement(ShaderAccessFlags access, uint64 size, uint64 offset, uint32 count) : DescriptorElement(access, BufferType::Uniform, count), size(size), offset(offset) {}
		uint64 size;
		uint64 offset;
	};

private:
	PipelineInfo* info;

	VkPipeline pipeline;
	VkRenderPass renderPass;
	VkPipelineLayout pipelineLayout;
	VkDescriptorSetLayout setLayout;
	VkDescriptorPool descriptorPool;
	VkDescriptorSet descriptorSet;

	buffer::UniformBuffer* uniformBuffer;

	utils::List<DescriptorElement*> descriptors;
	
	utils::List<VkFramebuffer> framebuffers;

	uint32 totalUniformBufferSize;

public:
	Pipeline(PipelineInfo* info);
	~Pipeline();

	void UpdateUniformBuffer(uint32 slot, const void* const data, uint64 offset, uint64 size) const;

	void SetUniformBuffer(const graphics::buffer::UniformBuffer* buffer, bool deleteOld = true);
	void SetTexture(uint32 slot, const graphics::texture::Texture* texture, const graphics::texture::Sampler* sampler) const;
	void SetTexture(uint32* slots, uint32 num, const graphics::texture::Texture*, const graphics::texture::Sampler* sampler) const;

	inline const buffer::Buffer* GetUniformBuffer() const { return uniformBuffer; }

	inline VkPipeline GetPipeline() const { return pipeline; }
	inline VkRenderPass GetRenderPass() const { return renderPass; }
	inline VkPipelineLayout GetPipelineLayout() const { return pipelineLayout; }
	inline const VkDescriptorSet& GetDescriptorSet() const { return descriptorSet; }

	inline const utils::List<VkFramebuffer>& GetFramebuffers() const { return framebuffers; }
	inline VkFramebuffer GetFramebuffer(uint_t index) const { return framebuffers[index]; }
};

#endif

} } }