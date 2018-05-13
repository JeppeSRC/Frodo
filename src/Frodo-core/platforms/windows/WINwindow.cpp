#include <core/video/window.h>
#include <core/video/context.h>
#include <core/log/log.h>
#include <core/event/eventdispatcher.h>

namespace fd {
namespace core {
namespace video {

using namespace log;
using namespace event;
using namespace math;

std::unordered_map<HWND, WINWindow*> WINWindow::windowHandles;

LRESULT WINWindow::WndProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l) {
	WINWindow* window = windowHandles[hwnd];

	if (!window) goto end;

	switch (msg) {
		case WM_CLOSE:
			window->open = false;
			break;
		case WM_MOVE: {
			WindowEvent e(EventAction::Move, vec2i(window->GetWidth(), window->GetHeight()), vec2i(LOWORD(l), HIWORD(l)), false, false);
			EventDispatcher::DispatchEvent(&e);
			}
			break;
		case WM_SIZE: {
			WindowEvent e(EventAction::Resize, vec2i(LOWORD(l), HIWORD(l)), vec2i(), false, false);
			window->info.width = e.size.x;
			window->info.height = e.size.y;
			EventDispatcher::DispatchEvent(&e);
			}
			break;
	}

	end:

	return DefWindowProc(hwnd, msg, w, l);
}

WINWindow::WINWindow(WindowCreateInfo* info) : Window(info) {

	WNDCLASSEX wnd = { 0 };

	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.hCursor = LoadCursor(0, IDC_ARROW);
	wnd.hIcon = LoadIcon(0, IDI_WINLOGO);
	wnd.lpfnWndProc = (WNDPROC)WndProc;
	wnd.lpszClassName = L"DANK";
	wnd.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wnd)) {
		Log::Fatal("[WINWindow] Failed to register class");
		return;
	}

	if (Factory::GetAdapters().Find(info->graphicsAdapter) == ~0) {
		FD_WARN("[WINWindow] Invalid adapter specified!");
		info->graphicsAdapter = Factory::GetAdapters()[0];
	}

	if (Factory::GetOutputs().Find(info->outputWindow) == ~0) {
		FD_WARN("[WINWindow] Invalid output specified!");
		info->outputWindow = Factory::GetOutputs()[0];
	}


	RECT coord = { 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };

#ifdef FD_DX
	DXGI_MODE_DESC mode = info->outputWindow->FindBestMatchingMode(info->width, info->height, info->refreshRate);
	info->outputWindow->SetMode(mode);

	info->width = mode.Width;
	info->height = mode.Height;
	info->refreshRate = (uint32)((float32)mode.RefreshRate.Numerator / mode.RefreshRate.Denominator);
	
	coord = info->outputWindow->GetDesktopCoordinates();
#endif

	uint32 monitorWidth = coord.right - coord.left;
	uint32 monitorHeight = coord.bottom - coord.top;

	uint32 xPos = coord.left + (monitorWidth >> 1) - (info->width >> 1);
	uint32 yPos = coord.top + (monitorHeight>> 1) - (info->height>> 1);

	RECT r = { 0, 0, (LONG)info->width, (LONG)info->height };

	AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);

	hwnd = CreateWindow(L"DANK", info->title.GetWCHAR(), WS_OVERLAPPEDWINDOW, xPos, yPos, r.right - r.left, r.bottom - r.top, 0, 0, 0, 0);
	Log::Debug("%u %u", r.right - r.left, r.bottom - r.top);
	if (!hwnd) {
		FD_FATAL("[WINWindow] Failed to create window! %u", GetLastError());
		return;
	}

	if (!Context::Init(this)) {
		DestroyWindow(hwnd);
		exit(1);
	}

	SetVisible(true);

	open = true;

	windowHandles[hwnd] = this;
}

WINWindow::~WINWindow() {
	Context::Dispose();

	DestroyWindow(hwnd);
}

void WINWindow::Update() const {

	MSG msg;

	if (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

}

void WINWindow::SetVisible(bool state) {
	ShowWindow(hwnd, state ? SW_SHOW : SW_HIDE);
}

void WINWindow::Resize(const WindowCreateInfo* const newInfo) {
	info = *newInfo;
	RECT r = { 0, 0, (LONG)info.width, (LONG)info.height };

	AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);

	SetWindowPos(hwnd, HWND_TOP, 0, 0, r.right - r.left, r.bottom - r.top, SWP_NOMOVE | SWP_SHOWWINDOW);
}

}
}
}