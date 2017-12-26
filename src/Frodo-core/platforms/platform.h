#pragma once


#ifdef FD_WINDOWS
#include <Windows.h>
#ifdef FD_DX
#include <d3d11.h>
#include <d3d10.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#elif defined(FD_VK)
#include <vulkan.h>
#include <vk_platform.h>
#include <vk_layer.h>
#endif
#endif