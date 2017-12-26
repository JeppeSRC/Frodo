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

#endif

} } }