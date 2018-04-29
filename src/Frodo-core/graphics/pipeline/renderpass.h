#pragma once

#include <platforms/platform.h>
#include <utils/list.h>
#include <graphics/texture/framebuffer.h>
#include <core/math/vec4.h>

#define FD_MAX_ATTACHMENTS 0x08
#define FD_NO_ATTACHMENT (~(uint32)0)
#define FD_SWAPCHAIN_ATTACHMENT_INDEX ((~(uint32)0) - 1)

namespace fd {
namespace graphics {
namespace pipeline {

struct RenderSubPassInfo {
	uint32 colorAttachments[FD_MAX_ATTACHMENTS];

	uint32 inputAttachments[FD_MAX_ATTACHMENTS];
};



struct RenderPassInfo {
	utils::List<texture::Framebuffer*> framebuffers;
	
	utils::List<RenderSubPassInfo> subpasses;

	uint32 depthAttachment;

	core::math::vec4 colorAttachmentClearColor[FD_MAX_ATTACHMENTS];
	float32 depthClearValue;
};

class RenderPass {
private:
	VkRenderPass renderPass;

	uint32 width;
	uint32 height;

	utils::List<texture::Framebuffer*> framebuffers;

	utils::List<VkFramebuffer> framebufferObjects;

	RenderPassInfo* info;
public:
	RenderPass();
	RenderPass(const RenderPassInfo* info);
	~RenderPass();

	void InitializeRenderPass(VkRenderPassBeginInfo* const info) const;

	inline uint32 GetWidth() const { return width; }
	inline uint32 GetHeight() const { return height; }

	inline VkRenderPass GetRenderPass() const { return renderPass; }
	inline VkFramebuffer GetFramebuffer(uint_t index) const { return framebufferObjects[index]; }
};



}
}
}