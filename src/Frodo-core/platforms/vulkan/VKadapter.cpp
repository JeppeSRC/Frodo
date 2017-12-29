#include <core/video/adapter.h>
#include <core/log/log.h>
#include <core/video/factory.h>
#include <core/video/window.h>

namespace fd {
namespace core {
namespace video {

using namespace utils;
using namespace log;

PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR Adapter::vkGetPhysicalDeviceWin32PresentationSupportKHR = nullptr;
PFN_vkCreateWin32SurfaceKHR Adapter::vkCreateWin32SurfaceKHR = nullptr;
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

		VkResult res = vkGetPhysicalDeviceDisplayPropertiesKHR(device, &numDisplays, nullptr);

		if (!numDisplays) {
			FD_WARN("[Adapter] No outputs connected to adapter \"%s\"", prop.deviceName);
			return;
		}

		VkDisplayPropertiesKHR* displays = new VkDisplayPropertiesKHR[numDisplays];

		vkGetPhysicalDeviceDisplayPropertiesKHR(device, &numDisplays, displays);

		for (uint32 i = 0; i < numDisplays; i++) {
			outputs.Push_back(new Output(displays[i], this));
		}

		delete[] displays;
	} else {
		outputs.Push_back(nullptr);
	}

	uint32 numExtensions = 0;

	vkEnumerateDeviceExtensionProperties(device, nullptr, &numExtensions, nullptr);
	VkExtensionProperties* extensions = new VkExtensionProperties[numExtensions];
	vkEnumerateDeviceExtensionProperties(device, nullptr, &numExtensions, extensions);

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
	return vkGetPhysicalDeviceWin32PresentationSupportKHR(device, queue);
}

uint32 Adapter::GetQueue(VkQueueFlags flag, bool supportsPresenting) const {
	if (flag != VK_QUEUE_GRAPHICS_BIT) {
		supportsPresenting = false;
		FD_WARN("[Adapter] Func: %s, supportsPresenting is only used with VK_QUEUE_GRAPHICS_BIT", __FUNCSIG__);
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

	vkCreateWin32SurfaceKHR(Factory::GetInstance(), &info, nullptr, &surface);

#else


#endif

	uint32 numFormats = 0;

	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &numFormats, nullptr);
	surfaceFormats.Resize(numFormats);
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &numFormats, surfaceFormats.GetData());

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

}
}
}