#include <graphics/texture/framebuffer.h>

namespace fd {
namespace graphics {
namespace texture {

Framebuffer::Framebuffer(uint32 width, uint32 height, Format format, VkImageUsageFlags usage, VkImageLayout layout) : Texture2D(width, height, (VkFormat)format, usage, layout) {}

Framebuffer::Framebuffer(uint32 width, uint32 height, Format format, bool usedAsInput) : Framebuffer(width, height, format, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | (usedAsInput ? (VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT) : 0), usedAsInput ? VK_IMAGE_LAYOUT_GENERAL : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) { }

Depthbuffer::Depthbuffer(uint32 width, uint32 height, Format format, bool usedAsInput) : Framebuffer(width, height, format, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | (usedAsInput ? (VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT) : 0), VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) { }
}
}
}