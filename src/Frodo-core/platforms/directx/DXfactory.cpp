#include <core/video/factory.h>
#include <core/log/log.h>

namespace fd {
namespace core {
namespace video {

using namespace utils;

IDXGIFactory* Factory::factory = nullptr;
List<Adapter*> Factory::adapters;
List<Output*> Factory::outputs;


void Factory::CreateFactory() {
	CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);

	FD_ASSERT(factory == nullptr);

	uint32 index = 0;
	IDXGIAdapter* adapter;

	while (factory->EnumAdapters(index++, &adapter) != DXGI_ERROR_NOT_FOUND) {
		adapters.Push_back(new Adapter(adapter));
	}

	FD_ASSERT(adapters.GetSize() == 0);

	for (uint_t i = 0; i < adapters.GetSize(); i++) {
		Adapter* adapter = adapters[i];
		List<Output*> tmp = adapter->GetOutputs();
		for (uint_t j = 0; j < tmp.GetSize(); j++) {
			outputs.Push_back(tmp[i]);
		}
	}
}

void Factory::Release() {
	factory->Release();
}


}}}