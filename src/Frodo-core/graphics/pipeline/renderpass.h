#pragma once

#include <platforms/platform.h>
#include <utils/list.h>
#include <graphics/texture/framebuffer.h>

namespace fd {
namespace graphics {
namespace pipeline {

#ifdef FD_DX

#else 

class RenderPass {
private:
	VkRenderPass renderPass;

	utils::List<texture::Framebuffer*> framebuffers;

	utils::List<VkFramebuffer> framebufferObjects;
public:
	RenderPass();
	~RenderPass();

	inline VkRenderPass GetRenderPass() const { return renderPass; }
	inline VkFramebuffer GetFramebuffer(uint_t index) const { return framebufferObjects[index]; }
};

#endif


}
}
}