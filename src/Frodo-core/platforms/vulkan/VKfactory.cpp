#include <core/video/factory.h>
#include <core/log/log.h>
#include <core/video/context.h>

namespace fd {
namespace core {
namespace video {

#ifdef FD_LINUX
#define __stdcall __attribute__((stdcall))
#endif

using namespace utils;
using namespace log;

VkInstance Factory::instance = nullptr;
List<Adapter*> Factory::adapters;
List<Output*> Factory::outputs;
List<const char*> Factory::instanceExtensions;
VkDebugReportCallbackEXT Factory::debug = nullptr;
PFN_vkCreateDebugReportCallbackEXT Factory::vkCreateDebugReportCallbackEXT = nullptr;
PFN_vkDestroyDebugReportCallbackEXT Factory::vkDestroyDebugReportCallbackEXT = nullptr;

static VkBool32 __stdcall debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64 object, uint_t location, int32 messageCode, const char* layerPrefix, const char* message, void* userData) {
	if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
		Log::Info("[Vulkan] %s", message);
	} else if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
		Log::Warning("[Vulkan] %s", message);
	} else if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
		Log::Warning("[Vulkan] %s", message);
	} else if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
		Log::Fatal("[Vulkan] %s", message);
	} else if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
		Log::Debug("[Vulkan] %s", message);
	}

	return VK_FALSE;
}

void Factory::CreateFactory() {
	VkApplicationInfo appInfo;

	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.apiVersion = 0;
	appInfo.applicationVersion = 1;
	appInfo.engineVersion = 1;
	appInfo.pApplicationName = "Frodo App";
	appInfo.pEngineName = "Frodo Engine";
	appInfo.pNext = nullptr;

	uint32 numExtensions = 0;
	
	VK(vkEnumerateInstanceExtensionProperties(nullptr, &numExtensions, nullptr));

	List<VkExtensionProperties> extensions(numExtensions);

	VK(vkEnumerateInstanceExtensionProperties(nullptr, &numExtensions, extensions.GetData()));
	
	instanceExtensions.Reserve(numExtensions);

	for (uint_t i = 0; i < numExtensions; i++) {
		uint_t len = strlen(extensions[i].extensionName);
		char* str = new char[len+1];
		memcpy(str, extensions[i].extensionName, len+1);

		instanceExtensions.Push_back(str);
	}

	const char* requiredExtensions[]{
		"VK_KHR_surface",
#ifdef FD_WINDOWS
		"VK_KHR_win32_surface"
#else
		"VK_KHR_xcb_surface"
#endif
	};

	if (IsExtensionsSupported(requiredExtensions, 2) != ~0) {
		FD_FATAL("[Factory] Required extensions are not supported. (VK_KHR_surface, %s)", requiredExtensions[1]);
	}

	VkInstanceCreateInfo instanceInfo;
	
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.enabledExtensionCount = numExtensions;
	instanceInfo.ppEnabledExtensionNames = instanceExtensions.GetData();
	instanceInfo.enabledLayerCount = 0;
	instanceInfo.ppEnabledLayerNames = nullptr;
	instanceInfo.pApplicationInfo = &appInfo;
	instanceInfo.flags = 0;
	instanceInfo.pNext = nullptr;
	
	if (VK(vkCreateInstance(&instanceInfo, nullptr, &instance)) != VK_SUCCESS) {
		FD_FATAL("[Factory] Failed to create VkInstance");
		return;
	}

	Adapter::InitFunctions();

	uint32 numDevices = 0;

	VK(vkEnumeratePhysicalDevices(instance, &numDevices, nullptr));

	VkPhysicalDevice* devices = new VkPhysicalDevice[numDevices];

	VK(vkEnumeratePhysicalDevices(instance, &numDevices, devices));

	for (uint32 i = 0; i < numDevices; i++) {
		adapters.Push_back(new Adapter(devices[i]));
		const List<Output*>& tmp = adapters[i]->GetOutputs();
		for (uint_t i = 0; i < tmp.GetSize(); i++) {
			outputs.Push_back(tmp[i]);
		}
	}

	delete[] devices;

	if (IsExtensionSupported("VK_EXT_debug_report")) {
		Factory::vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
		Factory::vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");

		VkDebugReportCallbackCreateInfoEXT info;

		info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		info.pNext = nullptr;
		info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT;
#ifdef FD_DEBUG
		info.flags |= VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_DEBUG_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_INFORMATION_BIT_EXT;
#endif

		info.pfnCallback = debugCallback;
		info.pUserData = nullptr;

		Factory::vkCreateDebugReportCallbackEXT(instance, &info, nullptr, &debug);

	} else {
		FD_WARN("[Factory] No debug reporting supported!");
	}

	

}

void Factory::Release() {
	Factory::vkDestroyDebugReportCallbackEXT(instance, debug, nullptr);
	vkDestroyInstance(instance, nullptr);
}

bool Factory::IsExtensionSupported(const char* name) {
	for (uint_t i = 0; i < instanceExtensions.GetSize(); i++) {
		if (strcmp(name, instanceExtensions[i]) == 0) {
			return true;
		}
	}

	return false;
}

uint32 Factory::IsExtensionsSupported(const char** names, uint32 num) {
	for (uint32 i = 0; i < num; i++) {
		if (!IsExtensionSupported(names[i])) return i;
	}

	return ~0;
}

} } }