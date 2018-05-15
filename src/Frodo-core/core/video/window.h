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

	utils::String title;

	Adapter* graphicsAdapter;
	Output* outputWindow;

	bool windowed;
};

class Window {
protected:
	WindowCreateInfo info;

	bool open;

	Window(WindowCreateInfo* info);
public:
	virtual ~Window() { }

	virtual void Update() const = 0;

	virtual void SetVisible(bool state) = 0;

	virtual void Resize(const WindowCreateInfo* const newInfo) = 0;

	inline uint32 GetWidth() const { return info.width; }
	inline uint32 GetHeight() const { return info.height; }
	inline utils::String GetTitle() const { return info.title; }
	inline const WindowCreateInfo* GetCreateInfo() const { return &info; }
	inline bool IsOpen() const { return open; }

	static Window* Create(WindowCreateInfo* info);
};

#ifdef FD_WINDOWS

class WINWindow : public Window {
private:
	HWND hwnd;

public:
	WINWindow(WindowCreateInfo* info);
	~WINWindow();

	void Update() const override;

	void SetVisible(bool state) override;

	void Resize(const WindowCreateInfo* const newInfo) override;

	inline HWND GetHandle() const { return hwnd; }
private:
	static std::unordered_map<HWND, WINWindow*> windowHandles;

	static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);

};

#elif FD_LINUX

#endif

} } }