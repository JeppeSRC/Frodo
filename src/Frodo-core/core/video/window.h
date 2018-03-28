#pragma once

#include <core/types.h>
#include <utils/string.h>
#include <core/video/output.h>
#include <core/video/factory.h>

#include <platforms/platform.h>

#include <map>
#include <unordered_map>

namespace fd {
namespace core {
namespace video {

struct WindowCreateInfo {
	uint32 width;
	uint32 height;
	uint32 refreshRate;

	utils::String title;

	Adapter* graphicsAdapter;
	Output* outputWindow;

	bool windowed;
};

class Window {
private:
#ifdef FD_WINDOWS
	HWND hwnd;
#elif FD_LINUX
	Display* display;
	Window window;
#endif
	WindowCreateInfo* info;

	bool open;
public:
	Window(WindowCreateInfo* info);
	
	void Update() const;

	void SetVisible(bool state);

	inline uint32 GetWidth() const { return info->width; }
	inline uint32 GetHeight() const { return info->height; }
	inline utils::String GetTitle() const { return info->title; }
	#ifdef FD_WINDOWS
	inline HWND GetHandle() const { return hwnd; }
	#elif FD_LINUX
	#endif
	inline const WindowCreateInfo* GetCreateInfo() const { return info; }
	inline bool IsOpen() const { return open; }


private:
#ifdef FD_WINDOWS
	static std::unordered_map<HWND, Window*> windowHandles;

	static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);
#endif
};

} } }