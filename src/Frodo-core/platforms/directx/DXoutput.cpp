#include <core/video/output.h>
#include <core/log/log.h>

namespace fd {
namespace core {
namespace video {

using namespace log;

Output::Output(IDXGIOutput* output) : output(output) {
	DXGI_OUTPUT_DESC desc;

	output->GetDesc(&desc);

	attachedToDesktop = desc.AttachedToDesktop;
	desktopCoordinates = desc.DesktopCoordinates;
	name = desc.DeviceName;
	monitor = desc.Monitor;

	switch (desc.Rotation) {
		case DXGI_MODE_ROTATION_UNSPECIFIED:
			orientation = MonitorOrientation::Unspecified;
			break;
		case DXGI_MODE_ROTATION_IDENTITY:
			orientation = MonitorOrientation::Identity;
			break;
		case DXGI_MODE_ROTATION_ROTATE90:
			orientation = MonitorOrientation::Rotate90;
			break;
		case DXGI_MODE_ROTATION_ROTATE180:
			orientation = MonitorOrientation::Rotate180;
			break;
		case DXGI_MODE_ROTATION_ROTATE270:
			orientation = MonitorOrientation::Rotate270;
			break;
	}

	uint32 numModes = 0;

	output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &numModes, nullptr);

	if (numModes == 0) {
		FD_WARN("[Output] No display modes available on output \"%s\"", *name);
		memset(&currentMode, 0, sizeof(DXGI_MODE_DESC));
		return;
	}

	modes.Resize(numModes);

	output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &numModes, modes.GetData());

	currentMode = GetBestMode();
}

Output::~Output() {
	output->Release();
}

DXGI_MODE_DESC Output::FindBestMatchingMode(uint32 width, uint32 height, uint32 refreshRate) const {
	if (width == 0 || height == 0) {
		return GetBestMode();
	}

	DXGI_MODE_DESC desc = { 0 };

	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Width = width;
	desc.Height = height;
	desc.RefreshRate.Numerator = refreshRate;
	desc.RefreshRate.Denominator = 1;
	
	DXGI_MODE_DESC bestMode;

	output->FindClosestMatchingMode(&desc, &bestMode, nullptr);
	

	return bestMode;

}

} } }