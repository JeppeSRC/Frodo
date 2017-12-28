#pragma once

#include <platforms/platform.h>
#include <utils/list.h>
#include "adapter.h"
#include "output.h"

namespace fd {
namespace core {
namespace video {


#ifdef FD_DX
class Factory {
private:
	static IDXGIFactory* factory;

	static utils::List<Adapter*> adapters;
	static utils::List<Output*> outputs;
public:
	static void CreateFactory();
	static void Release();

	inline static const utils::List<Adapter*>& GetAdapters() { return adapters; }
	inline static const utils::List<Output*>& GetOutputs() { return outputs; }

	inline static IDXGIFactory* GetFactory() { return factory; }
};

#else

class Factory {
private:
	static VkInstance instance;

	static utils::List<const char*> instanceExtensions;
	
	static utils::List<Adapter*> adapters;
	static utils::List<Output*> outputs;

	static VkDebugReportCallbackEXT debug;

	static PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT;
	static PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT;
public:
	static void CreateFactory();
	static void Release();

	static bool IsExtensionSupported(const char* name);
	static bool IsExtensionsSupported(const char** names, uint_t num);

	static inline VkInstance GetInstance() { return instance; }
	static inline const utils::List<Adapter*> GetAdapters() { return adapters; }
	static inline const utils::List<Output*> GetOutputs() { return outputs; }
};

#endif

} } }