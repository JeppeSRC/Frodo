#include <graphics/pipeline/pipeline.h>
#include <core/video/context.h>
#include <core/log/log.h>

namespace fd {
namespace graphics {
namespace pipeline {

using namespace utils;
using namespace buffer;
using namespace core;
using namespace video;
using namespace log;
using namespace texture;
using namespace event;

VkVertexInputBindingDescription* GetBindingDescriptors(const BufferLayout* layouts, uint32 num) {
	VkVertexInputBindingDescription* binding = new VkVertexInputBindingDescription[num];

	for (uint32 i = 0; i < num; i++) {
		VkVertexInputBindingDescription& b = binding[i];
		const BufferLayout& l = layouts[i];

		b.binding = l.GetInputSlot();
		b.inputRate = (VkVertexInputRate)l.GetInputRate();
		b.stride = l.GetStride();
	}

	return binding;
}

VkVertexInputAttributeDescription* GetAttributeDescriptors(const BufferLayout* layouts, uint32 num, uint32* numAttributes) {
	*numAttributes = 0;
	for (uint32 i = 0; i < num; i++) {
		const BufferLayout& l = layouts[i];
		*numAttributes += (uint32)l.GetAttribs().GetSize();
	}

	VkVertexInputAttributeDescription* attribs = new VkVertexInputAttributeDescription[*numAttributes];

	uint32 currentAttrib = 0;

	for (uint32 i = 0; i < num; i++) {
		const BufferLayout& l = layouts[i];
		const List<BufferLayoutAttrib>& attr = l.GetAttribs();

		uint32 currentOffset = 0;

		for (uint_t j = 0; j < attr.GetSize(); j++) {
			VkVertexInputAttributeDescription& a = attribs[currentAttrib];

			a.binding = l.GetInputSlot();
			a.location = currentAttrib++;
			a.offset = currentOffset;
			a.format = (VkFormat)attr[j].type;

			currentOffset += BufferLayout::GetSize(attr[j].type);
		}
	}

	return attribs;
}

bool VerifyPipelineInfo(PipelineInfo* const info) {

	if (info->numViewports == 0 || info->viewports == nullptr) {
		FD_FATAL("[Pipeline] No viewport(s) specified");
		return false;
	}

	if (info->numScissors == 0 || info->scissors == nullptr) {
		FD_FATAL("[Pipeline] No scissor(s) specified");
		return false;
	}

	switch (info->topology) {
		case PrimitiveTopology::PointList:
			break;
		case PrimitiveTopology::LineList:
			break;
		case PrimitiveTopology::LineStrip:
			break;
		case PrimitiveTopology::TriangleList:
			break;
		case PrimitiveTopology::TriangleStrip:
			break;
		default:
			FD_WARN("[Pipeline] No topology specified. Defaulting to TriangleList");
			info->topology = PrimitiveTopology::TriangleList;
				
	}

	switch (info->polygonMode) {
		case PolygonMode::Fill:
			break;
		case PolygonMode::Line:
			break;
		case PolygonMode::Point:
			break;
		default:
			FD_WARN("[Pipeline] No polygon mode specified. Defaulting to Fill");
			info->polygonMode = PolygonMode::Fill;
	}

	if (info->shader == nullptr) {
		FD_FATAL("[Pipeline] No shader specified");
		return false;
	}

	if (info->numInputLayouts == 0 || info->shaderInputLayouts == nullptr) {
		FD_FATAL("[Pipeline] no input layout(s) specified!");
		return false;
	}

	if (info->numBlends == 0 || info->blends == nullptr) {
		FD_FATAL("[Pipeline] No BlendInfo(s) specified");
		return false;
	}

	return true;
}


Pipeline::Pipeline(PipelineInfo* info, const RenderPass* const renderPass, uint32 subpassIndex, const PipelineLayout* const pipelineLayout, const Pipeline* const derivativePipeline) : EventListener(EventWindow), info(info) {

	if (!VerifyPipelineInfo(info)) {
		FD_FATAL("[Pipeline] Pipeline creation failed");
		return;
	}

	VkPipelineShaderStageCreateInfo shaderInfo[3];

	shaderInfo[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderInfo[0].flags = 0;
	shaderInfo[0].pNext = 0;
	shaderInfo[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	shaderInfo[0].pName = "main";
	shaderInfo[0].module = info->shader->GetVertexShader();
	shaderInfo[0].pSpecializationInfo = nullptr;

	shaderInfo[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderInfo[1].flags = 0;
	shaderInfo[1].pNext = 0;
	shaderInfo[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	shaderInfo[1].pName = "main";
	shaderInfo[1].module = info->shader->GetPixelShader();
	shaderInfo[1].pSpecializationInfo = nullptr;

	shaderInfo[2].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderInfo[2].flags = 0;
	shaderInfo[2].pNext = 0;
	shaderInfo[2].stage = VK_SHADER_STAGE_GEOMETRY_BIT;
	shaderInfo[2].pName = "main";
	shaderInfo[2].module = info->shader->GetGeometryShader();
	shaderInfo[2].pSpecializationInfo = nullptr;


	VkPipelineVertexInputStateCreateInfo vertexInfo;

	vertexInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInfo.pNext = nullptr;
	vertexInfo.flags = 0;
	vertexInfo.vertexBindingDescriptionCount = info->numInputLayouts;
	vertexInfo.pVertexBindingDescriptions = GetBindingDescriptors(info->shaderInputLayouts, info->numInputLayouts);
	vertexInfo.pVertexAttributeDescriptions = GetAttributeDescriptors(info->shaderInputLayouts, info->numInputLayouts, &vertexInfo.vertexAttributeDescriptionCount);
	
	VkPipelineInputAssemblyStateCreateInfo inputInfo;

	inputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputInfo.pNext = nullptr;
	inputInfo.flags = 0;
	inputInfo.primitiveRestartEnable = VK_FALSE;
	inputInfo.topology = (VkPrimitiveTopology)info->topology;

	viewports = new VkViewport[info->numViewports];

	for (uint32 i = 0; i < info->numViewports; i++) {
		VkViewport& v = viewports[i];
		const ViewportInfo& vi = info->viewports[i];

		v.x = vi.x;
		v.y = vi.y;
		v.width = vi.width;
		v.height = vi.height;
		v.maxDepth = vi.minDepth;
		v.maxDepth = vi.maxDepth;
	}

	scissors = new VkRect2D[info->numScissors];

	for (uint32 i = 0; i < info->numScissors; i++) {
		VkRect2D& s = scissors[i];
		const ScissorInfo& si = info->scissors[i];

		s.offset.x = si.x;
		s.offset.y = si.y;
		s.extent.width = si.width;
		s.extent.height = si.height;
	}
	
	VkPipelineViewportStateCreateInfo viewInfo;

	viewInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewInfo.pNext = nullptr;
	viewInfo.flags = 0;
	viewInfo.viewportCount = info->numViewports;
	viewInfo.pViewports = viewports;
	viewInfo.scissorCount = info->numScissors;
	viewInfo.pScissors = scissors;

	VkPipelineRasterizationStateCreateInfo rasterInfo;
	
	rasterInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterInfo.pNext = nullptr;
	rasterInfo.flags = 0;
	rasterInfo.lineWidth = 1.0f;
	rasterInfo.depthBiasEnable = VK_FALSE;
	rasterInfo.depthBiasClamp = 0;
	rasterInfo.depthBiasConstantFactor = 0;
	rasterInfo.depthBiasSlopeFactor = 0;
	rasterInfo.depthClampEnable = VK_FALSE;
	rasterInfo.cullMode = (VkCullModeFlags)info->cullMode;
	rasterInfo.frontFace = (VkFrontFace)info->frotFace;
	rasterInfo.rasterizerDiscardEnable = VK_FALSE;
	rasterInfo.polygonMode = (VkPolygonMode)info->polygonMode;

	VkPipelineMultisampleStateCreateInfo multisampleInfo;

	multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampleInfo.pNext = nullptr;
	multisampleInfo.flags = 0;
	multisampleInfo.sampleShadingEnable = VK_FALSE;
	multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampleInfo.minSampleShading = 1.0f;
	multisampleInfo.pSampleMask = nullptr;
	multisampleInfo.alphaToOneEnable = VK_FALSE;
	multisampleInfo.alphaToCoverageEnable = VK_FALSE;

	VkPipelineDepthStencilStateCreateInfo depthInfo;

	depthInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthInfo.pNext = nullptr;
	depthInfo.flags = 0;
	depthInfo.depthTestEnable = info->depthStencilInfo.depthEnable;
	depthInfo.depthCompareOp = (VkCompareOp)info->depthStencilInfo.depthFunc;
	depthInfo.stencilTestEnable = info->depthStencilInfo.stencilEnable;
	depthInfo.front.compareOp = (VkCompareOp)info->depthStencilInfo.frontFace.compare;
	depthInfo.front.depthFailOp = (VkStencilOp)info->depthStencilInfo.frontFace.depthFail;
	depthInfo.front.failOp = (VkStencilOp)info->depthStencilInfo.frontFace.stencilFail;
	depthInfo.front.passOp = (VkStencilOp)info->depthStencilInfo.frontFace.stencilPass;
	depthInfo.back.compareOp = (VkCompareOp)info->depthStencilInfo.backFace.compare;
	depthInfo.back.depthFailOp = (VkStencilOp)info->depthStencilInfo.backFace.depthFail;
	depthInfo.back.failOp = (VkStencilOp)info->depthStencilInfo.backFace.stencilFail;
	depthInfo.back.passOp = (VkStencilOp)info->depthStencilInfo.backFace.stencilPass;
	depthInfo.depthBoundsTestEnable = VK_FALSE;
	depthInfo.maxDepthBounds = 0.0f;
	depthInfo.minDepthBounds = 0.0f;

	VkPipelineColorBlendAttachmentState* blends = new VkPipelineColorBlendAttachmentState[info->numBlends];
	
	for (uint32 i = 0; i < info->numBlends; i++) {
		VkPipelineColorBlendAttachmentState& b = blends[i];
		const BlendInfo& bi = info->blends[i];

		b.blendEnable = bi.blendEnable;
		b.srcColorBlendFactor = (VkBlendFactor)bi.srcBlend;
		b.dstColorBlendFactor = (VkBlendFactor)bi.destBlend;
		b.colorBlendOp = (VkBlendOp)bi.blendOp;
		b.srcAlphaBlendFactor = (VkBlendFactor)bi.alpahSrcBlend;
		b.dstAlphaBlendFactor = (VkBlendFactor)bi.alphaDestBlend;
		b.alphaBlendOp = (VkBlendOp)bi.alphaBlendOp;
		b.colorWriteMask = bi.colorWriteMask;
	}

	VkPipelineColorBlendStateCreateInfo blendInfo;

	blendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	blendInfo.pNext = nullptr;
	blendInfo.flags = 0;
	blendInfo.logicOpEnable = VK_FALSE;
	blendInfo.logicOp = VK_LOGIC_OP_COPY;
	blendInfo.attachmentCount = info->numBlends;
	blendInfo.pAttachments = blends;

	VkPipelineDynamicStateCreateInfo dinfo;

	VkDynamicState state[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

	dinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dinfo.pNext = nullptr;
	dinfo.flags = 0;
	dinfo.dynamicStateCount = 2;
	dinfo.pDynamicStates = state;

	VkGraphicsPipelineCreateInfo pipeInfo;
	
	pipeInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipeInfo.pNext = nullptr;
	pipeInfo.flags = 0;
	pipeInfo.stageCount = 2;
	pipeInfo.pStages = shaderInfo;
	pipeInfo.pVertexInputState = &vertexInfo;
	pipeInfo.pInputAssemblyState = &inputInfo;
	pipeInfo.pTessellationState = nullptr;
	pipeInfo.pViewportState = &viewInfo;
	pipeInfo.pRasterizationState = &rasterInfo;
	pipeInfo.pMultisampleState = &multisampleInfo;
	pipeInfo.pDepthStencilState = &depthInfo;
	pipeInfo.pColorBlendState = &blendInfo;
	pipeInfo.pDynamicState = &dinfo;
	pipeInfo.layout = pipelineLayout->GetPipelineLayout();
	pipeInfo.renderPass = renderPass->GetRenderPass();
	pipeInfo.subpass = subpassIndex;
	pipeInfo.basePipelineHandle = derivativePipeline ? derivativePipeline->GetPipeline() : nullptr;
	pipeInfo.basePipelineIndex = -1;
	
	VK(vkCreateGraphicsPipelines(Context::GetDevice(), nullptr, 1, &pipeInfo, nullptr, &pipeline));
}

Pipeline::~Pipeline() {
	vkDestroyPipeline(Context::GetDevice(), pipeline, nullptr);
}

bool Pipeline::OnWindowEventResize(const core::math::vec2i& size) {
	viewports[0].width = size.x;
	viewports[0].height = size.y;

	scissors[0].extent.width= size.x;
	scissors[0].extent.height = size.y;

	return true;
}

}
}
}