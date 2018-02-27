#include <graphics/pipeline/renderpass.h>
#include <core/video/context.h>

namespace fd {
namespace graphics {
namespace pipeline {

using namespace core;
using namespace video;
using namespace utils;
using namespace texture;

RenderPass::RenderPass() : renderPass(nullptr) {

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

	VkSubpassDependency dependency;

	dependency.dependencyFlags = 0;
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcAccessMask = 0;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	VkRenderPassCreateInfo renderInfo;

	renderInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderInfo.pNext = nullptr;
	renderInfo.flags = 0;
	renderInfo.dependencyCount = 1;
	renderInfo.pDependencies = &dependency;
	renderInfo.attachmentCount = 1;
	renderInfo.pAttachments = &colorAttachment;
	renderInfo.subpassCount = 1;
	renderInfo.pSubpasses = &subDesc;

	VK(vkCreateRenderPass(Context::GetDevice(), &renderInfo, nullptr, &renderPass));

	const List<VkImageView>& swapchainViews = Context::GetSwapchainImageViews();

	for (uint_t i = 0; i < swapchainViews.GetSize(); i++) {
		VkFramebufferCreateInfo info;

		info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;
		info.width = Context::GetSwapchainExtent().width;
		info.height = Context::GetSwapchainExtent().height;
		info.layers = 1;
		info.renderPass = renderPass;
		info.attachmentCount = 1;
		info.pAttachments = &swapchainViews[i];

		VkFramebuffer tmp = nullptr;

		VK(vkCreateFramebuffer(Context::GetDevice(), &info, nullptr, &tmp));

		framebufferObjects.Push_back(tmp);
	}

}

RenderPass::~RenderPass() {
	for (uint_t i = 0; i < framebufferObjects.GetSize(); i++) {
		vkDestroyFramebuffer(Context::GetDevice(), framebufferObjects[i], nullptr);
	}

	vkDestroyRenderPass(Context::GetDevice(), renderPass, nullptr);
}

}
}
}