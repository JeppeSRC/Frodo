#pragma once

#include <platforms/platform.h>
#include <core/log/log.h>
#include <graphics/pipeline/pipeline.h>
#include <graphics/buffer/indexbuffer.h>
#include <graphics/buffer/vertexbuffer.h>
#include "adapter.h"

namespace fd {
namespace core {
namespace video {

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
	static VkCommandPool auxPool;

	static VkCommandBuffer auxCommandBuffer;

	static VkSemaphore imageSemaphore;
	static VkSemaphore renderSemaphore;

	static utils::List<VkImage> swapchainImages;
	static utils::List<VkImageView> swapchainViews;
	static utils::List<VkCommandBuffer> cmdbuffers;
	
	static Window* window;
	static Adapter* adapter;
	static Output* output;

private:
	static bool renderPassActive;
	static const graphics::buffer::IndexBuffer* currentIndexBuffer;

	static VkSubmitInfo submitInfo;
	static VkPresentInfoKHR presentInfo;

public:
	static void CopyBuffers(VkBuffer* dst, VkBuffer* src, uint64* size, uint64 num);
	static void TransitionImage(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	static void CopyBufferToImage(VkImage image, uint32 width, uint32 height, VkBuffer buffer);

public:
		static void Present();

public:
	static bool Init(Window* const window);
	static void Dispose();

	inline static const VkSwapchainKHR& GetSwapchain() { return swapChain; }
	inline static const VkDevice& GetDevice() { return device; }
	inline static const VkSurfaceKHR& GetSurface() { return surface; }
	
	inline static VkFormat GetSwapchainFormat() { return swapchainFormat; }
	inline static VkExtent2D GetSwapchainExtent() { return swapchainExtent; }
	
	inline static uint32 GetGraphicsQueueIndex() { return graphicsQueueIndex; }
	inline static uint32 GetPresentQueueIndex() { return presentQueueIndex; }

	inline static VkQueue GetGraphicsQueue() { return graphicsQueue; }
	inline static VkQueue GetPresentQueue() { return presentQueue; }

	inline static VkCommandPool GetCommandPool() { return cmdPool; }

	inline static const VkSemaphore& GetImageSemaphore() { return imageSemaphore; }
	inline static const VkSemaphore& GetRenderSemaphore() { return renderSemaphore; }

	inline static const utils::List<VkImageView>& GetSwapchainImageViews() { return swapchainViews; }
	inline static const utils::List<VkCommandBuffer>& GetCmdBuffers() { return cmdbuffers; }

	inline static Window* GetWindow() { return window; }
	inline static Adapter* GetAdapter() { return adapter; }
	inline static Output* GetOutputs() { return output; }
};

} } }