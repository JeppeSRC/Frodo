#pragma once

#include <platforms/platform.h>
#include <utils/string.h>
#include <utils/list.h>
#include "output.h"

namespace fd {
namespace core {
namespace video {

#ifdef FD_WINDOWS
typedef VkBool32(VKAPI_CALL *PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR)(VkPhysicalDevice physicalDevice, uint32 queueFamilyIndex);
typedef VkResult(VKAPI_CALL *PFN_vkCreateWin32SurfaceKHR)(VkInstance instance, const vkWin32SurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#elif FD_LINUX

#endif

class Window;

class Adapter {
private:
#ifdef FD_WINDOWS
	static PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR vkGetPhysicalDeviceWin32PresentationSupportKHR;
	static PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR;
#elif FD_LINUX

#endif
	static bool intialized;
	
public:
	static void InitFunctions();

private:
	VkPhysicalDevice device;

	VkPhysicalDeviceProperties prop;
	VkPhysicalDeviceMemoryProperties mem;

	VkSurfaceCapabilitiesKHR surfaceCababilities;

	utils::List<Output*> outputs;
	utils::List<VkQueueFamilyProperties> queueProperties;
	utils::List<VkSurfaceFormatKHR> surfaceFormats;
	utils::List<VkPresentModeKHR> presentModes;
	utils::List<const char*> deviceExtensions;
public:
	Adapter(VkPhysicalDevice device);
	~Adapter();

	bool SupportsPresenting(uint32 queue) const;
	uint32 GetQueue(VkQueueFlags flag, bool supportsPresenting = false) const;
	VkSurfaceKHR CreateSurface(const Window* window);
	bool IsExtensionSupported(const char* name) const;
	uint32 IsExtensionsSupported(const char** names, uint32 num) const;
	bool CheckImageFormat(VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* prop) const;

	inline VkPhysicalDevice GetPhysicalDevice() const { return device; }
	inline uint32 GetAPIVersion() const { return prop.apiVersion; }
	inline uint32 GetDriverVersion() const { return prop.driverVersion; }
	inline uint32 GetVendorID() const { return prop.vendorID; }
	inline uint32 GetDeviceID() const { return prop.deviceID; }
	inline VkPhysicalDeviceType GetDeviceType() const { return prop.deviceType; }
	inline utils::String GetName() const { return utils::String(prop.deviceName); }
	inline VkPhysicalDeviceLimits GetDeviceLimits() const { return prop.limits; }
	inline VkPhysicalDeviceSparseProperties GetDeviceSparesProperties() const { return prop.sparseProperties; }
	inline VkPhysicalDeviceMemoryProperties GetMemoryProperties() const { return mem; }
	inline VkSurfaceCapabilitiesKHR GetSurfaceCapabilities() const { return surfaceCababilities; }
	inline const utils::List<Output*>& GetOutputs() const { return outputs; }
	inline const utils::List<VkQueueFamilyProperties>& GetQueueProperties() const { return queueProperties; }
	inline const utils::List<VkSurfaceFormatKHR>& GetSurfaceFormats() const { return surfaceFormats; }
	inline const utils::List<VkPresentModeKHR>& GetPresentModes() const { return presentModes; }
	inline const utils::List<const char*>& GetDeviceExtensions()  { return deviceExtensions; }
};

}}}