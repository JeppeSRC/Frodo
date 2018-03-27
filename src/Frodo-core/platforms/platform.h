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

#include <xcb/xcb.h>

typedef VkFlags VkXcbSurfaceCreateFlagsKHR;

struct VkXcbSurfaceCreateInfoKHR {
	VkStructureType             sType;
	const void*                 pNext;
	VkXcbSurfaceCreateFlagsKHR  flags;
	xcb_connection_t*           connection;
	xcb_window_t                window;
};

#endif