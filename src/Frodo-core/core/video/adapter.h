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
	inline uint32 GetDedicatedVideoMemory() const { return vmem; }
	inline uint32 GetDedicatedSystemMemory() const { return smem; }
	inline uint32 GetSharedSystemMemory() const { return sharedsmem; }
	inline Output* GetFirstOutput() const { return outputs[0]; }
	inline const utils::List<Output*>& GetOutputs() const { return outputs; }
};

#endif


}}}