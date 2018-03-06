#pragma once

#include <platforms/platform.h>
#include <utils/list.h>
#include <graphics/texture/framebuffer.h>

#define FD_MAX_ATTACHMENTS 0x08
#define FD_NO_ATTACHMENT (~(uint32)0)
#define FD_SWAPCHAIN_ATTACHMENT_INDEX ((~(uint32)0) - 1)

namespace fd {
namespace graphics {
namespace pipeline {

struct RenderSubPassInfo {
	uint32 colorAttachments[FD_MAX_ATTACHMENTS];
	uint32 depthStencilAttachment;

	uint32 inputAttachments[FD_MAX_ATTACHMENTS];
};

struct RenderPassInfo {
	utils::List<texture::Framebuffer*> framebuffers;

	utils::List<RenderSubPassInfo> subpasses;


};

#ifdef FD_DX

#else 

class RenderPass {
private:
	VkRenderPass renderPass;

	utils::List<texture::Framebuffer*> framebuffers;

	utils::List<VkFramebuffer> framebufferObjects;

	RenderPassInfo* info;
public:
	RenderPass();
	RenderPass(const RenderPassInfo* info);
	~RenderPass();

	inline VkRenderPass GetRenderPass() const { return renderPass; }
	inline VkFramebuffer GetFramebuffer(uint_t index) const { return framebufferObjects[index]; }
};

#endif


}
}
}