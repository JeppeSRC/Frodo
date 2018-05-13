#include <graphics/pipeline/renderpass.h>
#include <core/video/context.h>
#include <core/log/log.h>

namespace fd {
namespace graphics {
namespace pipeline {

using namespace core;
using namespace video;
using namespace utils;
using namespace texture;
using namespace log;
using namespace math;
using namespace event;

static bool CheckFramebuffers(const List<Framebuffer*>& framebuffers) {

	uint32 w = framebuffers[0]->GetWidth();
	uint32 h = framebuffers[0]->GetHeight();

	for (uint_t i = 1; i < framebuffers.GetSize(); i++) {
		if (framebuffers[i]->GetWidth() != w) {
			return false;
		} else if (framebuffers[i]->GetHeight() != h) {
			return false;
		}
	}

	return true;
}

RenderPass::RenderPass(Format depthFormat) : EventListener(EventWindow), renderPass(nullptr), depthBuffer(nullptr) {

	VkExtent2D e = Context::GetSwapchainExtent();

	switch (depthFormat) {
		case Format::D16:
		case Format::D24S8:
		case Format::D32:
		case Format::D32S8:
			depthBuffer = new Depthbuffer(e.width, e.height, depthFormat);
	}

	VkAttachmentDescription attachments[2];

	attachments[0].flags = 0;
	attachments[0].format = Context::GetSwapchainFormat();
	attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;

	attachments[1].flags = 0;
	attachments[1].format = (VkFormat)depthFormat;
	attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;

	VkAttachmentReference ref;

	ref.attachment = 0;
	ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference dref;

	dref.attachment = 1;
	dref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

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
	subDesc.pDepthStencilAttachment = depthBuffer ? &dref : nullptr;

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
	renderInfo.attachmentCount = depthBuffer ? 2 : 1;
	renderInfo.pAttachments = attachments;
	renderInfo.subpassCount = 1;
	renderInfo.pSubpasses = &subDesc;

	VK(vkCreateRenderPass(Context::GetDevice(), &renderInfo, nullptr, &renderPass));

	const List<VkImageView>& swapchainViews = Context::GetSwapchainImageViews();

	VkImageView* views = new VkImageView[renderInfo.attachmentCount];

	clearValues.Reserve(2);

	VkClearValue clear;

	vec4 color(0, 0, 0, 0);

	memcpy(&clear.color, &color, sizeof(vec4));

	clearValues.Push_back(clear);

	if (depthBuffer) {
		views[1] = depthBuffer->GetImageView();

		clear.depthStencil.depth = 1.0f;

		clearValues.Push_back(clear);
	}

	for (uint_t i = 0; i < swapchainViews.GetSize(); i++) {
		views[0] = swapchainViews[i];

		finfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		finfo.pNext = nullptr;
		finfo.flags = 0;
		finfo.width = width = e.width;
		finfo.height = height = e.height;
		finfo.layers = 1;
		finfo.renderPass = renderPass;
		finfo.attachmentCount = renderInfo.attachmentCount;
		finfo.pAttachments = views;

		VkFramebuffer tmp = nullptr;

		VK(vkCreateFramebuffer(Context::GetDevice(), &finfo, nullptr, &tmp));

		framebufferObjects.Push_back(tmp);
	}

}

RenderPass::RenderPass(const RenderPassInfo* info) : EventListener(EventWindow, false), renderPass(nullptr), info(new RenderPassInfo), framebuffers(info->framebuffers) {
	*this->info = *info;

	VkAttachmentDescription* attachments = new VkAttachmentDescription[framebuffers.GetSize() + 1];

	uint32 swapchainIndex = framebuffers.GetSize();

	VkAttachmentDescription& swapchainAttachment = attachments[swapchainIndex];

	swapchainAttachment.flags = 0;
	swapchainAttachment.format = Context::GetSwapchainFormat();
	swapchainAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	swapchainAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	swapchainAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	swapchainAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	swapchainAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	swapchainAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	swapchainAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

	for (uint_t i = 0; i < info->framebuffers.GetSize(); i++) {
		VkAttachmentDescription& desc = attachments[i];

		desc.flags = 0;
		desc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		desc.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		desc.format = info->framebuffers[i]->GetFormat();
		desc.samples = VK_SAMPLE_COUNT_1_BIT;
		desc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		desc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		desc.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	}

	List<VkAttachmentReference> attachmentReferences;
	List<VkSubpassDescription> subpassDescriptions;

	attachmentReferences.Reserve(info->subpasses.GetSize() * FD_MAX_ATTACHMENTS + 1);

	usesSwapchainImage = false;

	if (info->depthAttachment != FD_NO_ATTACHMENT) {
		VkAttachmentReference ref;

		ref.attachment = info->depthAttachment;
		ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		attachmentReferences.Push_back(ref);
	}

	for (uint_t i = 0; i < info->subpasses.GetSize(); i++) {
		RenderSubPassInfo subInfo = info->subpasses[i];
		
		VkSubpassDescription subpass;

		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.flags = 0;
		subpass.colorAttachmentCount = ~0;
		subpass.inputAttachmentCount = ~0;
		subpass.preserveAttachmentCount = 0;
		subpass.pInputAttachments = nullptr;
		subpass.pDepthStencilAttachment = nullptr;
		subpass.pPreserveAttachments = nullptr;
		subpass.pResolveAttachments = nullptr;

		VkAttachmentReference ref;

		ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		for (uint32 j = 0; j < FD_MAX_ATTACHMENTS; j++) {
			ref.attachment = subInfo.colorAttachments[j];

			if (ref.attachment == FD_NO_ATTACHMENT) {
				subpass.colorAttachmentCount = j;
				break;
			} else if (ref.attachment == FD_SWAPCHAIN_ATTACHMENT_INDEX) {
				ref.attachment = swapchainIndex;
				usesSwapchainImage = true;
			}

			if (ref.attachment == info->depthAttachment) {
				Log::Fatal("[RenderPass] Color attachment can't use the same framebuffer as depth/stencil attachment");
				return;
			}

			attachmentReferences.Push_back(ref);
		}

		if (subpass.colorAttachmentCount == ~0) {
			subpass.colorAttachmentCount = FD_MAX_ATTACHMENTS;
		} else if (subpass.colorAttachmentCount == 0) {
			Log::Fatal("[RenderPass] No color attachment specified!");
			return;
		}

		subpass.pColorAttachments = &attachmentReferences[attachmentReferences.GetSize() - subpass.colorAttachmentCount];

		ref.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		for (uint32 j = 0; j < FD_MAX_ATTACHMENTS; j++) {
			ref.attachment = subInfo.inputAttachments[j];

			if (ref.attachment == FD_NO_ATTACHMENT) {
				subpass.inputAttachmentCount = j;
				break;
			}

			attachmentReferences.Push_back(ref);
		}

		if (subpass.inputAttachmentCount == ~0) {
			subpass.inputAttachmentCount = FD_MAX_ATTACHMENTS;
		}

		if (subpass.inputAttachmentCount != 0) subpass.pInputAttachments = &attachmentReferences[attachmentReferences.GetSize() - subpass.inputAttachmentCount];

		if (info->depthAttachment != FD_NO_ATTACHMENT) {
			subpass.pDepthStencilAttachment = &attachmentReferences[0];
		}
		
		subpassDescriptions.Push_back(subpass);
	}

	clearValues.Resize(framebuffers.GetSize());

	for (uint_t i = 0; i < framebuffers.GetSize(); i++) {
		if (i == info->depthAttachment) {
			clearValues[i].depthStencil.depth = info->depthClearValue;
		} else {
			memcpy(&clearValues[i].color, &info->clearColor, sizeof(vec4));
		}
	}

	if (usesSwapchainImage) {
		clearValues.Resize(clearValues.GetSize() + 1);
		memcpy(&clearValues[swapchainIndex].color, &info->clearColor, sizeof(vec4));
	}

	List<VkSubpassDependency> dependencies;

	VkSubpassDependency dep;

	dep.srcSubpass = VK_SUBPASS_EXTERNAL;
	dep.dstSubpass = 1;
	dep.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dep.srcAccessMask = 0;
	dep.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dep.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dep.dependencyFlags = 0;
	
	dependencies.Push_back(dep);

	for (uint32 i = 1; i < subpassDescriptions.GetSize(); i++) {
		dep.srcSubpass = i - 1;
		dep.dstSubpass = i;

		dependencies.Push_back(dep);
	}

	VkRenderPassCreateInfo rinfo;

	rinfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	rinfo.pNext = nullptr;
	rinfo.flags = 0;
	rinfo.attachmentCount = info->framebuffers.GetSize() + (usesSwapchainImage ?  1 : 0);
	rinfo.pAttachments = attachments;
	rinfo.subpassCount = subpassDescriptions.GetSize();
	rinfo.pSubpasses = &subpassDescriptions[0];
	rinfo.dependencyCount = dependencies.GetSize();
	rinfo.pDependencies = &dependencies[0];

	VK(vkCreateRenderPass(Context::GetDevice(), &rinfo, nullptr, &renderPass));

	VkImageView* imageViews = new VkImageView[framebuffers.GetSize() + 1];

	for (uint_t i = 0; i < framebuffers.GetSize(); i++) {
		imageViews[i] = framebuffers[i]->GetImageView();
	}

	finfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	finfo.flags = 0;
	finfo.layers = 1;
	finfo.renderPass = renderPass;
	finfo.pAttachments = imageViews;
	
	if (usesSwapchainImage) {
		finfo.width = width = Context::GetSwapchainExtent().width;
		finfo.height = height = Context::GetSwapchainExtent().height;
		finfo.attachmentCount = rinfo.attachmentCount;

		List<VkImageView> swapViews = Context::GetSwapchainImageViews();

		for (uint_t i = 0; i < swapViews.GetSize(); i++) {
			imageViews[swapchainIndex] = swapViews[i];

			VkFramebuffer framebuffer;

			VK(vkCreateFramebuffer(Context::GetDevice(), &finfo, nullptr, &framebuffer));

			framebufferObjects.Push_back(framebuffer);
		}

		Register();

	} else {
		CheckFramebuffers(framebuffers);
		finfo.width = width = framebuffers[0]->GetWidth();
		finfo.height = height = framebuffers[0]->GetHeight();
		finfo.attachmentCount = framebuffers.GetSize();

		VkFramebuffer framebuffer;

		VK(vkCreateFramebuffer(Context::GetDevice(), &finfo, nullptr, &framebuffer));

		framebufferObjects.Push_back(framebuffer);
	}

}

RenderPass::~RenderPass() {
	for (uint_t i = 0; i < framebufferObjects.GetSize(); i++) {
		vkDestroyFramebuffer(Context::GetDevice(), framebufferObjects[i], nullptr);
	}

	vkDestroyRenderPass(Context::GetDevice(), renderPass, nullptr);

	delete[] finfo.pAttachments;
	delete info;
}

void RenderPass::InitializeRenderPass(VkRenderPassBeginInfo* const info) const {
	info->clearValueCount = clearValues.GetSize();
	info->pClearValues = clearValues.GetData();
}

bool RenderPass::OnWindowEventResize(const vec2i& size) {
	if (info == nullptr) {
		for (uint_t i = 0; i < framebufferObjects.GetSize(); i++) {
			vkDestroyFramebuffer(Context::GetDevice(), framebufferObjects[i], nullptr);
		}

		framebufferObjects.Clear();

		VkImageView* views = (VkImageView*)finfo.pAttachments;

		const List<VkImageView>& swapchainViews = Context::GetSwapchainImageViews();

		if (depthBuffer) {
			depthBuffer->Resize(size.x, size.y);
			views[1] = depthBuffer->GetImageView();
		}

		for (uint_t i = 0; i < swapchainViews.GetSize(); i++) {
			views[0] = swapchainViews[i];

			finfo.width = width = size.x;
			finfo.height = height = size.y;

			VkFramebuffer tmp = nullptr;

			VK(vkCreateFramebuffer(Context::GetDevice(), &finfo, nullptr, &tmp));

			framebufferObjects.Push_back(tmp);
		}
	} else {
		for (uint_t i = 0; i < framebufferObjects.GetSize(); i++) {
			vkDestroyFramebuffer(Context::GetDevice(), framebufferObjects[i], nullptr);
		}

		framebufferObjects.Clear();

		finfo.width = width = size.x;
		finfo.height = height = size.y;

		List<VkImageView> swapViews = Context::GetSwapchainImageViews();

		uint64 swapchainIndex = framebuffers.GetSize();

		VkImageView* imageViews = (VkImageView*)finfo.pAttachments;

		for (uint_t i = 0; i < framebuffers.GetSize(); i++) {
			framebuffers[i]->Resize(size.x, size.y);
			imageViews[i] = framebuffers[i]->GetImageView();
		}

		for (uint_t i = 0; i < swapViews.GetSize(); i++) {
			imageViews[swapchainIndex] = swapViews[i];

			VkFramebuffer framebuffer;

			VK(vkCreateFramebuffer(Context::GetDevice(), &finfo, nullptr, &framebuffer));

			framebufferObjects.Push_back(framebuffer);
		}
	}

	return true;
}

}
}
}