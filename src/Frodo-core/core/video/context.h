#pragma once

#include <platforms/platform.h>
#include <core/log/log.h>
#include "adapter.h"

namespace fd {
namespace core {
namespace video {

#ifdef FD_DX

class Context {
private:
	friend class Window;
	friend struct PipelineInfo;

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
	static void InitPipeline(PipelineInfo* pipeInfo, uint32 num);
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

#ifdef FD_DEBUG
#define VK(dankFunction) VkFunctionLogBullshit(dankFunction, __FILE__, #dankFunction, __FUNCTION__, __LINE__)
#else
#define VK(dankFunction) dankFunction
#endif


static VkResult VkFunctionLogBullshit(VkResult res, const char* const file, const char* const callingFunc, const char* const func, uint32 line) {
	if (res != VK_SUCCESS) {
		log::Log::Fatal("[Vulkan] Error %d calling \"%s\" in %s -> %s -> %u", res, callingFunc, file, func, line);
	}

	return res;
}


class Context {
private:
	friend class Window;
	friend struct PipelineInfo;

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

	static VkCommandPool cmdPool;

	static VkSemaphore imageSemaphore;
	static VkSemaphore renderSemaphore;

	static utils::List<VkImage> swapchainImages;
	static utils::List<VkImageView> swapchainViews;
	static utils::List<VkCommandBuffer> cmdbuffers;
	
	static Window* window;
	static Adapter* adapter;
	static Output* output;

public:
	static bool Init(Window* window);
	static void Dispose();

	inline static VkSwapchainKHR GetSwapchain() { return swapChain; }
	inline static VkDevice GetDevice() { return device; }
	inline static VkSurfaceKHR GetSurface() { return surface; }
	
	inline static VkFormat GetSwapchainFormat() { return swapchainFormat; }
	inline static VkExtent2D GetSwapchainExtent() { return swapchainExtent; }
	
	inline static uint32 GetGraphicsQueueIndex() { return graphicsQueueIndex; }
	inline static uint32 GetPresentQueueIndex() { return presentQueueIndex; }

	inline static VkQueue GetGraphicsQueue() { return graphicsQueue; }
	inline static VkQueue GetPresentQueue() { return presentQueue; }

	inline static VkSemaphore GetImageSemaphore() { return imageSemaphore; }
	inline static VkSemaphore GetRenderSemaphore() { return renderSemaphore; }

	inline static const utils::List<VkImageView>& GetImageViews() { return swapchainViews; }

	inline static Window* GetWindow() { return window; }
	inline static Adapter* GetAdapter() { return adapter; }
	inline static Output* GetOutputs() { return output; }
};

#endif

} } }