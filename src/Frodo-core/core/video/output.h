#pragma once

#include <platforms/platform.h>
#include <core/enums.h>
#include <utils/string.h>
#include <utils/list.h>

namespace fd {
namespace core {
namespace video {

#ifdef FD_DX

class Output {
private:
	IDXGIOutput* output;

	utils::String name;

	utils::List<DXGI_MODE_DESC> modes;
	DXGI_MODE_DESC currentMode;
	
	bool attachedToDesktop;
	FD_MONITOR_ORIENTATION orientation;
	RECT desktopCoordinates;
	HMONITOR monitor;

public:
	Output(IDXGIOutput* output);
	~Output();

	inline IDXGIOutput* GetOutput() const { return output; }
	inline utils::String GetName() const { return name; }
	inline DXGI_MODE_DESC GetBestMode() const { return modes[modes.GetSize() - 1]; }
	inline const utils::List<DXGI_MODE_DESC>& GetModes() const { return modes; }
	inline DXGI_MODE_DESC GetCurrentMode() const { return currentMode; }
	inline bool IsAttachedToDesktop() const { return attachedToDesktop; }
	inline FD_MONITOR_ORIENTATION GetOrientation() const { return orientation; }
	inline HMONITOR GetHMONITOR() const { return monitor; }

	inline void SetMode(DXGI_MODE_DESC desc) { this->currentMode = desc; }
};

#endif

}
}
}