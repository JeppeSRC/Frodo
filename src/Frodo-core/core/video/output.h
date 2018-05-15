#pragma once

#include <platforms/platform.h>
#include <core/enums.h>
#include <utils/string.h>
#include <utils/list.h>

namespace fd {
namespace core {
namespace video {

class Adapter;

class Output {
private:
	VkDisplayPropertiesKHR prop;

	utils::List<VkDisplayModePropertiesKHR> modes;

	Adapter* adapter;
public:
	Output(VkDisplayPropertiesKHR prop, Adapter* adapter);
	~Output();

	inline VkDisplayKHR GetDisplay() const { return prop.display; }
	inline utils::String GetName() const { return utils::String(prop.displayName); }
};

}
}
}