#pragma once

#include <platforms/platform.h>
#include "adapter.h"

namespace fd {
namespace core {
namespace video {

#ifdef FD_DX

class Context {
private:
	friend class Window;

private:
	static ID3D11Device* device;
	static ID3D11DeviceContext* deviceContext;
	static ID3D11RenderTargetView* targetView;
	static ID3D11DepthStencilView* depthView;
	static IDXGISwapChain* swapChain;

	static Window* window;
	static Adapter* adapter;
	static Output* output;

public:

	static bool Init(Window* window);
	static void Dispose();

	static void SetFullscreen(bool state);
	static void Present(uint32 syncInterval, uint32 flags);
	static void Clear();

	__forceinline static ID3D11Device* GetDevice() { return device; }
	__forceinline static ID3D11DeviceContext* GetDeviceContext() { return deviceContext; }
	
	__forceinline static Window* GetWindow() { return window; }
	__forceinline static Adapter* GetAdapter() { return adapter; }
	__forceinline static Output* GetOutput() { return output; }
};

#else

class Context {
private:
	friend class Window;

private:
	static VkSwapchainKHR swapChain;
	static VkDevice device;
	static VkSurfaceKHR surface;

	static VkFormat swapchainFormat;
	static VkExtent2D swapchainExtent;

	static uint32 graphicsQueueIndex;
	static uint32 presentQueueIndex;

	static VkQueue graphicsQueue;
	static VkQueue presentQueue;

	static utils::List<VkImage> swapchainImages;
	static utils::List<VkImageView> swapchainViews;
	
	static Window* window;
	static Adapter* adapter;
	static Output* output;

public:
	static bool Init(Window* window);
	static void Dispose();
};

#endif

} } }