#pragma once
#include <graphics/buffer/bufferlayout.h>
#include <graphics/shader/shader.h>
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

struct BlendInfo {
	bool blendEnable;
	BlendFactor srcBlend;
	BlendFactor destBlend;
	BlendOp blendOp;
	BlendFactor alpahSrcBlend;
	BlendFactor alphaDestBlend;
	BlendOp alphaBlendOp;
	uint8 colorWriteMask;
};

struct PipelineLayoutElement {
	const utils::String name;
	BufferType type;
	uint32 id;
	uint32 size;
	uint32 count;
	uint32 shaderAccess;
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

	uint32 numBufferLayouts;
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
	PipelineInfo* info;

	VkPipeline pipeline;
	VkRenderPass renderPass;
	VkPipelineLayout pipelineLayout;

public:
	Pipeline(PipelineInfo* info);
	~Pipeline();


};

#endif

} } }