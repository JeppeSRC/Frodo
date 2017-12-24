#include <core/video/output.h>
#include <core/log/log.h>

namespace fd {
namespace core {
namespace video {

Output::Output(IDXGIOutput* output) : output(output) {
	DXGI_OUTPUT_DESC desc;

	output->GetDesc(&desc);

	attachedToDesktop = desc.AttachedToDesktop;
	desktopCoordinates = desc.DesktopCoordinates;
	name = desc.DeviceName;
	monitor = desc.Monitor;

	switch (desc.Rotation) {
		case DXGI_MODE_ROTATION_UNSPECIFIED:
			orientation = FD_MONITOR_ORIENTATION::UNSPECIFIED;
			break;
		case DXGI_MODE_ROTATION_IDENTITY:
			orientation = FD_MONITOR_ORIENTATION::IDENTITY;
			break;
		case DXGI_MODE_ROTATION_ROTATE90:
			orientation = FD_MONITOR_ORIENTATION::ROTATE90;
			break;
		case DXGI_MODE_ROTATION_ROTATE180:
			orientation = FD_MONITOR_ORIENTATION::ROTATE180;
			break;
		case DXGI_MODE_ROTATION_ROTATE270:
			orientation = FD_MONITOR_ORIENTATION::ROTATE270;
			break;
	}

	uint32 numModes = 0;

	output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &numModes, nullptr);

	if (numModes == 0) {
		FD_WARN("[Output] No display modes available on output \"%s\"", *name);
	}

	DXGI_MODE_DESC* descs = new DXGI_MODE_DESC[numModes];

	output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &numModes, descs);

	for (uint32 i = 0; i < numModes; i++) {
		modes.Push_back(descs[i]);
	}

	delete[] descs;

	currentMode = GetBestMode();
}

Output::~Output() {
	output->Release();
}

} } }