#include <core/video/context.h>
#include <core/video/window.h>
#include <core/log/log.h>

namespace fd {
namespace core {
namespace video {

using namespace log;
using namespace utils;

VkSwapchainKHR Context::swapChain = nullptr;
VkDevice Context::device = nullptr;
VkSurfaceKHR Context::surface = nullptr;

VkFormat Context::swapchainFormat;
VkExtent2D Context::swapchainExtent;

uint32 Context::graphicsQueueIndex = ~0;
uint32 Context::presentQueueIndex = ~0;

VkQueue Context::graphicsQueue;
VkQueue Context::presentQueue;

List<VkImage> Context::swapchainImages;
List<VkImageView> Context::swapchainViews;

Window* Context::window = nullptr;
Adapter* Context::adapter = nullptr;
Output* Context::output = nullptr;

bool Context::Init(Window* window) {
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

	if (vkCreateSwapchainKHR(device, &sinfo, nullptr, &swapChain) != VK_SUCCESS) {
		FD_FATAL("[Context] Swapchain creation failed even when you did all those checks you nuub!");
		return false;
	}

	uint32 numImages = 0;

	vkGetSwapchainImagesKHR(device, swapChain, &numImages, nullptr);
	swapchainImages.Resize(numImages);
	vkGetSwapchainImagesKHR(device, swapChain, &numImages, swapchainImages.GetData());

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
		vkCreateImageView(device, &vinfo, nullptr, &view);

		swapchainViews.Push_back(view);
	}



	return true;
}

void Context::Dispose() {
	vkDestroySurfaceKHR(Factory::GetInstance(), surface, nullptr);
	vkDestroyDevice(device, nullptr);
	vkDestroySwapchainKHR(device, swapChain, nullptr);
}

} } }