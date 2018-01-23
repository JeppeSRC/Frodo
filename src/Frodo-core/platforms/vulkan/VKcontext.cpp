#include <core/video/context.h>
#include <core/video/window.h>
#include <core/log/log.h>
#include <graphics/pipeline/pipeline.h>

namespace fd {
namespace core {
namespace video {

using namespace log;
using namespace utils;
using namespace graphics;
using namespace pipeline;
using namespace buffer;

VkSwapchainKHR Context::swapChain = nullptr;
VkDevice Context::device = nullptr;
VkSurfaceKHR Context::surface = nullptr;

VkFormat Context::swapchainFormat;
VkExtent2D Context::swapchainExtent;

uint32 Context::graphicsQueueIndex = ~0;
uint32 Context::presentQueueIndex = ~0;

VkQueue Context::graphicsQueue;
VkQueue Context::presentQueue;

VkCommandPool Context::cmdPool;
VkCommandPool Context::auxPool;

VkCommandBuffer Context::auxCommandBuffer;

VkSemaphore Context::imageSemaphore;
VkSemaphore Context::renderSemaphore;

List<VkImage> Context::swapchainImages;
List<VkImageView> Context::swapchainViews;
List<VkCommandBuffer> Context::cmdbuffers;

Window* Context::window = nullptr;
Adapter* Context::adapter = nullptr;
Output* Context::output = nullptr;

const Pipeline* Context::currentRenderPass = nullptr;
const IndexBuffer* Context::currentIndexBuffer = nullptr;

VkSubmitInfo Context::submitInfo;
VkPresentInfoKHR Context::presentInfo;

static const VkPipelineStageFlags st[]{ VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

bool Context::Init(Window* const window) {
	Context::window = window;
	Context::adapter = window->GetCreateInfo()->graphicsAdapter;
	Context::output = window->GetCreateInfo()->outputWindow;

	uint32 numQueues = 1;

	graphicsQueueIndex = adapter->GetQueue(VK_QUEUE_GRAPHICS_BIT, true);

	if (graphicsQueueIndex == ~0) {
		graphicsQueueIndex = adapter->GetQueue(VK_QUEUE_GRAPHICS_BIT, false);
		presentQueueIndex = adapter->GetQueue(0, true);

		numQueues++;
	}

	VkQueueFamilyProperties queueProperties = adapter->GetQueueProperties()[graphicsQueueIndex];

	VkDeviceQueueCreateInfo qinfo[2];

	float32 prio = 1.0f;

	qinfo[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	qinfo[0].pNext = nullptr;
	qinfo[0].flags = 0;
	qinfo[0].pQueuePriorities = &prio;
	qinfo[0].queueCount = queueProperties.queueCount;
	qinfo[0].queueFamilyIndex = graphicsQueueIndex;

	if (presentQueueIndex != ~0) {
		queueProperties = adapter->GetQueueProperties()[presentQueueIndex];

		qinfo[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		qinfo[1].pNext = nullptr;
		qinfo[1].flags = 0;
		qinfo[1].pQueuePriorities = &prio;
		qinfo[1].queueCount = queueProperties.queueCount;
		qinfo[1].queueFamilyIndex = presentQueueIndex;
	}

	if (!adapter->IsExtensionSupported("VK_KHR_swapchain")) {
		FD_FATAL("[Context] Adapter \"%s\" doesn't support extension VK_KHR_swapchain!", *adapter->GetName());
		return false;
	}

	VkDeviceCreateInfo deviceInfo;

	deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.pNext = nullptr;
	deviceInfo.flags = 0;
	deviceInfo.enabledLayerCount = 0;
	deviceInfo.ppEnabledLayerNames = nullptr;
	deviceInfo.enabledExtensionCount = (uint32)adapter->GetDeviceExtensions().GetSize();
	deviceInfo.ppEnabledExtensionNames = adapter->GetDeviceExtensions().GetData();
	deviceInfo.queueCreateInfoCount = numQueues;
	deviceInfo.pQueueCreateInfos = qinfo;
	deviceInfo.pEnabledFeatures = nullptr;

	if (vkCreateDevice(adapter->GetPhysicalDevice(), &deviceInfo, nullptr, &device) != VK_SUCCESS) {
		FD_FATAL("[Context] Failed to create device!");
		return false;
	}

	surface = adapter->CreateSurface(window);

	vkGetDeviceQueue(device, graphicsQueueIndex, 0, &graphicsQueue);

	if (presentQueueIndex != ~0) {
		vkGetDeviceQueue(device, presentQueueIndex, 0, &presentQueue);
	} else {
		presentQueue = graphicsQueue;
	}

	const List<VkSurfaceFormatKHR>& surfaceFormats = adapter->GetSurfaceFormats();

	swapchainFormat  = VK_FORMAT_UNDEFINED;
	VkColorSpaceKHR swapchainColorspace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;

	if (surfaceFormats.GetSize() == 1 && surfaceFormats[0].format == VK_FORMAT_UNDEFINED) {
		swapchainFormat = VK_FORMAT_R8G8B8A8_UNORM;
	} else {
		for (uint_t i = 0; i < surfaceFormats.GetSize(); i++) {
			if (surfaceFormats[i].format == VK_FORMAT_R8G8B8A8_UNORM) {
				swapchainFormat = VK_FORMAT_R8G8B8A8_UNORM;
				break;
			}
		}

		if (swapchainFormat == VK_FORMAT_UNDEFINED) {
			for (uint_t i = 0; i < surfaceFormats.GetSize(); i++) {
				if (surfaceFormats[i].format == VK_FORMAT_B8G8R8A8_UNORM) {
					swapchainFormat = VK_FORMAT_B8G8R8A8_UNORM;
					break;
				}
			}
		}
	}

	const List<VkPresentModeKHR> presentModes = adapter->GetPresentModes();

	VkPresentModeKHR preferredModes[]{
		VK_PRESENT_MODE_IMMEDIATE_KHR,
		VK_PRESENT_MODE_MAILBOX_KHR,
		VK_PRESENT_MODE_FIFO_RELAXED_KHR,
		VK_PRESENT_MODE_FIFO_KHR
	};

	VkPresentModeKHR presentMode = VK_PRESENT_MODE_MAX_ENUM_KHR;

	for (uint32 i = 0; i < 4; i++) {
		if (presentModes.Find(preferredModes[i]) != ~0) {
			presentMode = preferredModes[i];
			break;
		}
	}

	VkSurfaceCapabilitiesKHR capabilities = adapter->GetSurfaceCapabilities();

	swapchainExtent = capabilities.currentExtent;

	if (swapchainExtent.width == -1 && swapchainExtent.height == -1) {
		swapchainExtent = { window->GetWidth(), window->GetHeight() };
	} 

	uint32 imageCount = capabilities.minImageCount > 0 ? capabilities.minImageCount : capabilities.minImageCount + 1;

	if (imageCount > capabilities.maxImageCount) imageCount = capabilities.maxImageCount;

	const uint32 queueIndices[]{
		graphicsQueueIndex,
		presentQueueIndex
	};

	VkSwapchainCreateInfoKHR sinfo;

	sinfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	sinfo.pNext = nullptr;
	sinfo.flags = 0;
	sinfo.imageArrayLayers = 1;
	sinfo.imageExtent = swapchainExtent;
	sinfo.imageFormat = swapchainFormat;
	sinfo.minImageCount = imageCount;
	sinfo.imageColorSpace = swapchainColorspace;
	sinfo.surface = surface;
	sinfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	sinfo.imageSharingMode = presentQueueIndex == ~0 ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT;
	sinfo.queueFamilyIndexCount = presentQueueIndex == ~0 ? 1 : 2;
	sinfo.pQueueFamilyIndices = queueIndices;
	sinfo.presentMode = presentMode;
	sinfo.preTransform = capabilities.currentTransform;
	sinfo.oldSwapchain = nullptr;
	sinfo.clipped = VK_TRUE;
	sinfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	if (VK(vkCreateSwapchainKHR(device, &sinfo, nullptr, &swapChain)) != VK_SUCCESS) {
		FD_FATAL("[Context] Swapchain creation failed even when you did all those checks you nuub!");
		return false;
	}

	uint32 numImages = 0;

	VK(vkGetSwapchainImagesKHR(device, swapChain, &numImages, nullptr));
	swapchainImages.Resize(numImages);
	VK(vkGetSwapchainImagesKHR(device, swapChain, &numImages, swapchainImages.GetData()));

	VkImageViewCreateInfo vinfo;

	vinfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	vinfo.pNext = nullptr;
	vinfo.flags = 0;
	vinfo.format = swapchainFormat;
	vinfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	vinfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	vinfo.subresourceRange.levelCount = 1;
	vinfo.subresourceRange.layerCount = 1;
	vinfo.subresourceRange.baseMipLevel = 0;
	vinfo.subresourceRange.baseArrayLayer = 0;
	vinfo.components.r = VK_COMPONENT_SWIZZLE_R;
	vinfo.components.g = VK_COMPONENT_SWIZZLE_G;
	vinfo.components.b = VK_COMPONENT_SWIZZLE_B;
	vinfo.components.a = VK_COMPONENT_SWIZZLE_A;

	for (uint_t i = 0; i < swapchainImages.GetSize(); i++) {
		vinfo.image = swapchainImages[i];

		VkImageView view;
		VK(vkCreateImageView(device, &vinfo, nullptr, &view));

		swapchainViews.Push_back(view);
	}

	VkCommandPoolCreateInfo poolInfo;

	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.pNext = nullptr;
	poolInfo.flags = 0;
	poolInfo.queueFamilyIndex = graphicsQueueIndex;
	
	VK(vkCreateCommandPool(device, &poolInfo, nullptr, &cmdPool));
	VK(vkCreateCommandPool(device, &poolInfo, nullptr, &auxPool));

	cmdbuffers.Resize(swapchainImages.GetSize());

	VkCommandBufferAllocateInfo cmdInfo;

	cmdInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmdInfo.pNext = nullptr;
	cmdInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cmdInfo.commandBufferCount = (uint32)cmdbuffers.GetSize();
	cmdInfo.commandPool = cmdPool;

	VK(vkAllocateCommandBuffers(device, &cmdInfo, cmdbuffers.GetData()));

	cmdInfo.commandBufferCount = 1;
	cmdInfo.commandPool = auxPool;

	VK(vkAllocateCommandBuffers(device, &cmdInfo, &auxCommandBuffer));

	VkSemaphoreCreateInfo semInfo;

	semInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semInfo.pNext = nullptr;
	semInfo.flags = 0;

	VK(vkCreateSemaphore(Context::GetDevice(), &semInfo, nullptr, &imageSemaphore));
	VK(vkCreateSemaphore(Context::GetDevice(), &semInfo, nullptr, &renderSemaphore));

	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = nullptr;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &imageSemaphore;
	submitInfo.pWaitDstStageMask = st;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &renderSemaphore;
	submitInfo.commandBufferCount = 1;

	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pNext = nullptr;
	presentInfo.pResults = nullptr;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &swapChain;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &renderSemaphore;

	return true;
}

void Context::Dispose() {
	vkDestroySemaphore(Context::GetDevice(), imageSemaphore, nullptr);
	vkDestroySemaphore(Context::GetDevice(), renderSemaphore, nullptr);

	vkDestroyCommandPool(device, cmdPool, nullptr);
	vkDestroyCommandPool(device, auxPool, nullptr);

	for (uint_t i = 0; i < swapchainViews.GetSize(); i++) {
		vkDestroyImageView(Context::GetDevice(), swapchainViews[i], nullptr);
	}

	vkDestroySurfaceKHR(Factory::GetInstance(), surface, nullptr);
	vkDestroyDevice(device, nullptr);
	vkDestroySwapchainKHR(device, swapChain, nullptr);
}

void Context::CopyBuffers(VkBuffer dst, VkBuffer src, uint64 size) {
	VkCommandBufferBeginInfo info;

	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	info.pNext = nullptr;
	info.pInheritanceInfo = nullptr;
	info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	VK(vkBeginCommandBuffer(auxCommandBuffer, &info));

	VkBufferCopy cinfo;

	cinfo.dstOffset = 0;
	cinfo.srcOffset = 0;
	cinfo.size = size;

	vkCmdCopyBuffer(auxCommandBuffer, src, dst, 1, &cinfo);

	VK(vkEndCommandBuffer(auxCommandBuffer));

	VkSubmitInfo sinfo;

	sinfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	sinfo.pNext = nullptr;
	sinfo.commandBufferCount = 1;
	sinfo.pCommandBuffers = &auxCommandBuffer;
	sinfo.signalSemaphoreCount = 0;
	sinfo.waitSemaphoreCount = 0;

	VK(vkQueueSubmit(graphicsQueue, 1, &sinfo, nullptr));
	VK(vkQueueWaitIdle(graphicsQueue));

}

void Context::BeginCommandBuffers() {
	VkCommandBufferBeginInfo info;

	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	info.pNext = nullptr;
	info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
	info.pInheritanceInfo = nullptr;

	for (uint_t i = 0; i < cmdbuffers.GetSize(); i++) {
		VK(vkBeginCommandBuffer(cmdbuffers[i], &info));
	}
}

void Context::EndCommandBuffers() {
	if (currentRenderPass) {
		EndRenderPass();
	}

	for (uint_t i = 0; i < cmdbuffers.GetSize(); i++) {
		VK(vkEndCommandBuffer(cmdbuffers[i]));
	}
}

static VkClearValue gay{ 0.0f, 0.0f, 0.0f, 0.0f };

void Context::BeginRenderPass(const Pipeline* const pipeline) {

	VkRenderPassBeginInfo rinfo;

	rinfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	rinfo.pNext = nullptr;
	rinfo.renderArea.offset = { 0, 0 };
	rinfo.renderArea.extent = swapchainExtent;
	rinfo.renderPass = pipeline->GetRenderPass();
	rinfo.clearValueCount = 1;
	rinfo.pClearValues = &gay;

	if (currentRenderPass) {
		EndRenderPass();
	}

	for (uint_t i = 0; i < cmdbuffers.GetSize(); i++) {
		rinfo.framebuffer = pipeline->GetFramebuffer(i);

		vkCmdBeginRenderPass(cmdbuffers[i], &rinfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(cmdbuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetPipeline());
	}

	currentRenderPass = pipeline;
}

void Context::EndRenderPass() {
	for (uint_t i = 0; i < cmdbuffers.GetSize(); i++) {
		vkCmdEndRenderPass(cmdbuffers[i]);
	}

	currentRenderPass = nullptr;
}

void Context::Bind(const VertexBuffer* const buffer, uint32 slot) {
	uint64 offset = 0;
	for (uint_t i = 0; i < cmdbuffers.GetSize(); i++) {
		vkCmdBindVertexBuffers(cmdbuffers[i], slot, 1, &buffer->GetBuffer(), &offset);
	}
}

void Context::Bind(const IndexBuffer* const buffer) {
	for (uint_t i = 0; i < cmdbuffers.GetSize(); i++) {
		vkCmdBindIndexBuffer(cmdbuffers[i], buffer->GetBuffer(), 0, buffer->GetFormat());
	}

	currentIndexBuffer = buffer;
}

void Context::UpdateUniform(uint32 slot, const void* const data, uint64 offset, uint64 size) {
	currentRenderPass->UpdateUniformBuffer(slot, data, offset, size);
}

void Context::DrawIndexed() {
	for (uint_t i = 0; i < cmdbuffers.GetSize(); i++) {
		vkCmdDrawIndexed(cmdbuffers[i], currentIndexBuffer->GetCount(), 1, 0, 0, 0);
	}
}

void Context::Present() {
	uint32 imageIndex;

	VK(vkAcquireNextImageKHR(device, swapChain, ~0L, imageSemaphore, nullptr, &imageIndex));

	submitInfo.pCommandBuffers = &cmdbuffers[imageIndex];

	VK(vkQueueSubmit(graphicsQueue, 1, &submitInfo, nullptr));

	presentInfo.pImageIndices = &imageIndex;
	
	VK(vkQueuePresentKHR(presentQueue, &presentInfo));
}

} } }