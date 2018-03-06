#include <graphics/texture/framebuffer.h>

namespace fd {
namespace graphics {
namespace texture {

Framebuffer::Framebuffer(uint32 width, uint32 height, Format format) : Texture2D(width, height, (VkFormat)format, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) { }

}
}
}