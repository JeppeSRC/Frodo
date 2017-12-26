#include <core/video/adapter.h>
#include <core/log/log.h>

namespace fd {
namespace core {
namespace video {

using namespace utils;

Adapter::Adapter(IDXGIAdapter* adapter) : adapter(adapter) {

	DXGI_ADAPTER_DESC desc;

	adapter->GetDesc(&desc);

	subSytemID = desc.SubSysId;
	revision = desc.Revision;

	vmem = desc.DedicatedVideoMemory;
	smem = desc.DedicatedSystemMemory;
	sharedsmem = desc.SharedSystemMemory;

	name = desc.Description;


	uint32 index = 0;
	IDXGIOutput* tmp = nullptr;

	while (adapter->EnumOutputs(index++, &tmp) != DXGI_ERROR_NOT_FOUND) {
		outputs.Push_back(new Output(tmp));
	}

	if (outputs.GetSize() == 0) {
		FD_WARN("[Adapter] no outputs connnected to adpater \"%s\"", *name);
	}
}

Adapter::~Adapter() {
	adapter->Release();
}

}}}