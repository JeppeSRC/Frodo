#include <core/video/adapter.h>
#include <core/log/log.h>
#include <core/video/factory.h>
#include <core/video/window.h>
#include <core/video/context.h>

namespace fd {
namespace core {
namespace video {

using namespace utils;
using namespace log;

#ifdef FD_WINDOWS
PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR Adapter::vkGetPhysicalDeviceWin32PresentationSupportKHR = nullptr;
PFN_vkCreateWin32SurfaceKHR Adapter::vkCreateWin32SurfaceKHR = nullptr;
#elif FD_LINUX

#endif

bool Adapter::intialized = false;

void Adapter::InitFunctions() {
	if (intialized) return;
	
#ifdef FD_WINDOWS
	vkGetPhysicalDeviceWin32PresentationSupportKHR = (PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR)vkGetInstanceProcAddr(Factory::GetInstance(), "vkGetPhysicalDeviceWin32PresentationSupportKHR");
	vkCreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)vkGetInstanceProcAddr(Factory::GetInstance(), "vkCreateWin32SurfaceKHR");
#endif

	intialized = true;
}

Adapter::Adapter(VkPhysicalDevice device) : device(device) {

	vkGetPhysicalDeviceProperties(device, &prop);
	vkGetPhysicalDeviceMemoryProperties(device, &mem);

	uint32 numQueues = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &numQueues, nullptr);
	queueProperties.Resize(numQueues);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &numQueues, queueProperties.GetData());

	uint32 numFormats = 0;

	if (Factory::IsExtensionSupported("VK_KHR_display")) {

		uint32 numDisplays = 0;

		VK(vkGetPhysicalDeviceDisplayPropertiesKHR(device, &numDisplays, nullptr));

		if (!numDisplays) {
			FD_WARN("[Adapter] No outputs connected to adapter \"%s\"", prop.deviceName);
			return;
		}

		VkDisplayPropertiesKHR* displays = new VkDisplayPropertiesKHR[numDisplays];

		VK(vkGetPhysicalDeviceDisplayPropertiesKHR(device, &numDisplays, displays));

		for (uint32 i = 0; i < numDisplays; i++) {
			outputs.Push_back(new Output(displays[i], this));
		}

		delete[] displays;
	} else {
		outputs.Push_back(nullptr);
	}

	uint32 numExtensions = 0;

	VK(vkEnumerateDeviceExtensionProperties(device, nullptr, &numExtensions, nullptr));
	VkExtensionProperties* extensions = new VkExtensionProperties[numExtensions];
	VK(vkEnumerateDeviceExtensionProperties(device, nullptr, &numExtensions, extensions));

	for (uint32 i = 0; i < numExtensions; i++) {
		uint_t len = strlen(extensions[i].extensionName);
		char* str = new char[len+1];
		memcpy(str, extensions[i].extensionName, len+1);

		deviceExtensions.Push_back(str);
	}

	delete[] extensions;

	FD_DBG("[Adapter] Found adapter \"%s\"", prop.deviceName);

}

Adapter::~Adapter() {

}

bool Adapter::SupportsPresenting(uint32 queue) const {
#ifdef FD_WINDOWS
	return vkGetPhysicalDeviceWin32PresentationSupportKHR(device, queue);
#elif FD_LINUX

#endif

	return false;
}

uint32 Adapter::GetQueue(VkQueueFlags flag, bool supportsPresenting) const {
	if (flag != VK_QUEUE_GRAPHICS_BIT) {
		supportsPresenting = false;
		FD_WARN("[Adapter] Func: %s, supportsPresenting is only used with VK_QUEUE_GRAPHICS_BIT", FUNCSIG);
	}

	for (uint_t i = 0; queueProperties.GetSize(); i++) {
		VkQueueFamilyProperties p = queueProperties[i];

		if (p.queueFlags & flag && p.queueCount > 0) {
			if (supportsPresenting) {
				if (SupportsPresenting((uint32)i)) {
					return (uint32)i;
				}
			} else {
				return (uint32)i;
			}
		}
	}

	if (!flag) {
		for (uint_t i = 0; queueProperties.GetSize(); i++) {
			VkQueueFamilyProperties p = queueProperties[i];
			
			if (p.queueCount > 0 && SupportsPresenting((uint32)i)) return (uint32)i;
		}
	}

	return ~0;
}

VkSurfaceKHR Adapter::CreateSurface(const Window* window) {
	VkSurfaceKHR surface;

#ifdef FD_WINDOWS
	vkWin32SurfaceCreateInfoKHR info;

	info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	info.pNext = nullptr;
	info.instance = 0;
	info.hwnd = window->GetHandle();

	if (vkCreateWin32SurfaceKHR(Factory::GetInstance(), &info, nullptr, &surface) != VK_SUCCESS) {
		FD_FATAL("[Adapter] Failed to create surface on device \"%s\"", *GetName());
	}

#elif FD_LINUX


#endif

	surfaceFormats.Clear();

	uint32 num = 0;

	VK(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &num, nullptr));
	surfaceFormats.Resize(num);
	VK(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &num, surfaceFormats.GetData()));

	VK(vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &num, nullptr));
	presentModes.Resize(num);
	VK(vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &num, presentModes.GetData()));

	VK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &surfaceCababilities));

	return surface;
}

bool Adapter::IsExtensionSupported(const char* name) const {
	for (uint_t i = 0; i < deviceExtensions.GetSize(); i++) {
		if (strcmp(name, deviceExtensions[i]) == 0) {
			return true;
		}
	}

	return false;
}

uint32 Adapter::IsExtensionsSupported(const char** names, uint32 num) const {
	for (uint32 i = 0; i < num; i++) {
		if (!IsExtensionSupported(names[i])) return i;
	}

	return ~0;
}

bool Adapter::CheckImageFormat(VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* prop) const {
	VkResult res = vkGetPhysicalDeviceImageFormatProperties(device, format, type, tiling, usage, flags, prop);
	if (res == VK_ERROR_FORMAT_NOT_SUPPORTED) {
		return false;
	}

	return true;
}

}
}
}