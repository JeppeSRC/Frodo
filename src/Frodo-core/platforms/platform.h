#pragma once

#ifdef FD_DX
#include <d3d11.h>
#include <d3d10.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#elif FD_VK
#include <vulkan.h>
#include <vk_platform.h>
#include <vk_layer.h>
#endif

#ifdef FD_WINDOWS
#include <Windows.h>

typedef VkFlags VkWin32SurfaceCreateFlagsKHR;

struct vkWin32SurfaceCreateInfoKHR {
	VkStructureType sType;
	const void* pNext;
	VkWin32SurfaceCreateFlagsKHR flags;
	HINSTANCE instance;
	HWND hwnd;
};


#elif FD_LINUX

#define XLIB_ILLEGAL_ACCESS
#include <X11/Xlib.h>

#ifdef None
#undef None
#endif

#ifdef Always
#undef Always
#endif

#ifdef Never
#undef Never
#endif
/*
typedef VkFlags VkXcbSurfaceCreateFlagsKHR;

struct VkXcbSurfaceCreateInfoKHR {
	VkStructureType             sType;
	const void*                 pNext;
	VkXcbSurfaceCreateFlagsKHR  flags;
	xcb_connection_t*           connection;
	xcb_window_t                window;
};*/

typedef VkFlags VkXlibSurfaceCreateFlagsKHR;

struct VkXlibSurfaceCreateInfoKHR {
    VkStructureType                sType;
    const void*                    pNext;
    VkXlibSurfaceCreateFlagsKHR    flags;
    Display*                       dpy;
    Window                         window;
};

#endif