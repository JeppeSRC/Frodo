#include <core/video/output.h>
#include <core/video/adapter.h>
#include <core/log/log.h>
#include <core/video/context.h>

namespace fd {
namespace core {
namespace video {

using namespace log;

Output::Output(VkDisplayPropertiesKHR prop, Adapter* adpater) : prop(prop), adapter(adapter) {

	uint32 numModes = 0;

	VK(vkGetDisplayModePropertiesKHR(adpater->GetPhysicalDevice(), prop.display, &numModes, nullptr));

	if (!numModes) {
		FD_FATAL("[Output] No modes available on output \"%s\"", prop.displayName);
		return;
	}

	VkDisplayModePropertiesKHR* mode = new VkDisplayModePropertiesKHR[numModes];

	VK(vkGetDisplayModePropertiesKHR(adapter->GetPhysicalDevice(), prop.display, &numModes, mode));

	for (uint32 i = 0; i < numModes; i++) {
		modes.Push_back(mode[i]);
	}

	delete[] mode;
}

}
}
}