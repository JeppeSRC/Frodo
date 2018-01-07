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
		*numAttributes += l.GetAttribs().GetSize();
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

			currentOffset += BufferLayout::GetSize(attr[i].type);
		}
	}

	return attribs;
}

VkDescriptorSetLayout GetDescriptorSetLayout(const PipelineLayout& layout) {
	VkDescriptorSetLayout setLayout = nullptr;

	if (layout.numElements == 0) {
		return nullptr;
	}

	VkDescriptorSetLayoutBinding* binding = new VkDescriptorSetLayoutBinding[layout.numElements];

	for (uint32 i = 0; i < layout.numElements; i++) {
		VkDescriptorSetLayoutBinding& b = binding[i];
		const PipelineLayoutElement& e = layout.elements[i];

		b.binding = 0;
		b.descriptorCount = e.count;
		b.descriptorType = (VkDescriptorType)e.type;
		b.pImmutableSamplers = 0;
		b.stageFlags = (VkShaderStageFlags)e.shaderAccess;
	}

	VkDescriptorSetLayoutCreateInfo info;

	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.bindingCount = layout.numElements;
	info.pBindings = binding;

	vkCreateDescriptorSetLayout(Context::GetDevice(), &info, nullptr, &setLayout);

	return setLayout;
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

	if (info->pipelineLayout.numElements == 0 || info->pipelineLayout.elements == nullptr) {
		FD_WARN("[Pipeline] No pipeline layout specified");
	}

	FD_WARN("[Pipeline] DeptStencilInfo fucking noob!");

	if (info->numBlends == 0 || info->blends == nullptr) {
		FD_FATAL("[Pipeline] No BlendInfo(s) specified");
		return false;
	}

	return true;
}


Pipeline::Pipeline(PipelineInfo* info) : info(info) {

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

	VkViewport* viewports = new VkViewport[info->numViewports];

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

	VkRect2D* scissors = new VkRect2D[info->numScissors];

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

	VkPipelineLayoutCreateInfo layoutInfo;

	VkDescriptorSetLayout setLayout = GetDescriptorSetLayout(info->pipelineLayout);;

	layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	layoutInfo.pNext = nullptr;
	layoutInfo.flags = 0;
	layoutInfo.setLayoutCount = setLayout ? 1 : 0;
	layoutInfo.pSetLayouts = &setLayout;
	layoutInfo.pushConstantRangeCount = 0;
	layoutInfo.pPushConstantRanges = 0;

	vkCreatePipelineLayout(Context::GetDevice(), &layoutInfo, nullptr, &pipelineLayout);

	VkAttachmentDescription colorAttachment;

	colorAttachment.flags = 0;
	colorAttachment.format = Context::GetSwapchainFormat();
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

	VkAttachmentReference ref;

	ref.attachment = 0;
	ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subDesc;

	subDesc.flags = 0;
	subDesc.colorAttachmentCount = 1;
	subDesc.pColorAttachments = &ref;
	subDesc.inputAttachmentCount = 0;
	subDesc.pInputAttachments = nullptr;
	subDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subDesc.pResolveAttachments = nullptr;
	subDesc.preserveAttachmentCount = 0;
	subDesc.pPreserveAttachments = nullptr;
	subDesc.pDepthStencilAttachment = nullptr;

	VkRenderPassCreateInfo renderInfo;

	renderInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderInfo.pNext = nullptr;
	renderInfo.flags = 0;
	renderInfo.dependencyCount = 0;
	renderInfo.pDependencies = nullptr;
	renderInfo.attachmentCount = 1;
	renderInfo.pAttachments = &colorAttachment;
	renderInfo.subpassCount = 1;
	renderInfo.pSubpasses = &subDesc;

	vkCreateRenderPass(Context::GetDevice(), &renderInfo, nullptr, &renderPass);

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
	pipeInfo.pDynamicState = nullptr;
	pipeInfo.layout = pipelineLayout;
	pipeInfo.renderPass = renderPass;
	pipeInfo.subpass = 0;
	pipeInfo.basePipelineHandle = nullptr;
	pipeInfo.basePipelineIndex = -1;

	VkResult res = vkCreateGraphicsPipelines(Context::GetDevice(), nullptr, 1, &pipeInfo, nullptr, &pipeline);
	
}

Pipeline::~Pipeline() {
	vkDestroyPipeline(Context::GetDevice(), pipeline, nullptr);
	vkDestroyRenderPass(Context::GetDevice(), renderPass, nullptr);
	vkDestroyPipelineLayout(Context::GetDevice(), pipelineLayout, nullptr);
}

}
}
}