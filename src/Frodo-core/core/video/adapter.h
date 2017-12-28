#pragma once

#include <platforms/platform.h>
#include <utils/string.h>
#include <utils/list.h>
#include "output.h"

namespace fd {
namespace core {
namespace video {

#ifdef FD_DX
class Adapter {
private:
	IDXGIAdapter* adapter;

	uint32 subSytemID;
	uint32 revision;

	uint_t vmem;
	uint_t smem;
	uint_t sharedsmem;

	utils::String name;

	uint32 vendorID;
	uint32 deviceID;

	utils::List<Output*> outputs;

public:
	Adapter(IDXGIAdapter* adapter);
	~Adapter();

	inline IDXGIAdapter* GetAdapater() const { return adapter; }
	inline uint32 GetSubSystemID() const { return subSytemID; }
	inline uint32 GetRevision() const { return revision; }
	inline uint_t GetDedicatedVideoMemory() const { return vmem; }
	inline uint_t GetDedicatedSystemMemory() const { return smem; }
	inline uint_t GetSharedSystemMemory() const { return sharedsmem; }
	inline utils::String GetName() const { return name; }
	inline Output* GetFirstOutput() const { return outputs[0]; }
	inline const utils::List<Output*>& GetOutputs() const { return outputs; }
};

#else

#ifdef FD_WINDOWS
typedef VkBool32(VKAPI_CALL *PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR)(VkPhysicalDevice physicalDevice, uint32 queueFamilyIndex);
typedef VkResult(VKAPI_CALL *PFN_vkCreateWin32SurfaceKHR)(VkInstance instance, const vkWin32SurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);

#endif
class Window;

class Adapter {
private:
	static PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR vkGetPhysicalDeviceWin32PresentationSupportKHR;
	static PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR;
	static bool intialized;
	
public:
	static void InitFunctions();

private:
	VkPhysicalDevice device;

	VkPhysicalDeviceProperties prop;
	VkPhysicalDeviceMemoryProperties mem;

	utils::List<Output*> outputs;
	utils::List<VkQueueFamilyProperties> queueProperties;
	utils::List<VkSurfaceFormatKHR> surfaceFormats;
	utils::List<const char*> deviceExtensions;
public:
	Adapter(VkPhysicalDevice device);
	~Adapter();

	bool SupportsPresenting(uint32 queue) const;
	uint32 GetQueue(VkQueueFlags flag, bool supportsPresenting = false) const;
	VkSurfaceKHR CreateSurface(const Window* window);

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
	inline const utils::List<Output*>& GetOutputs() const { return outputs; }
	inline const utils::List<VkQueueFamilyProperties>& GetQueueProperties() const { return queueProperties; }
	inline const utils::List<VkSurfaceFormatKHR>& GetSurfaceFormats() const { return surfaceFormats; }
	inline const utils::List<const char*>& GetDeviceExtensions() const { return deviceExtensions; }
};

#endif


}}}