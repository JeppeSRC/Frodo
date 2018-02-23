#include <graphics/texture/texture.h>
#include <core/video/context.h>

namespace fd {
namespace graphics {
namespace texture {

using namespace FD;
using namespace utils;
using namespace buffer;
using namespace core::video;
	
Texture2D::Texture2D(const String& filename) {
	Header header;
	TextureHeader texHeader;

	byte* pixels = nullptr;

	LoadImageFile(filename, &header, &texHeader, &pixels);

	if (pixels == nullptr) return;

	width = texHeader.width;
	height = texHeader.height;

	VkFormat format = VK_FORMAT_UNDEFINED;

	switch (texHeader.pixelLayout) {
		case TextureChannel::R:
			format = VK_FORMAT_R8_UNORM;
		case TextureChannel::RG:
			format = VK_FORMAT_R8G8_UNORM;
		case TextureChannel::RGBA:
			format = VK_FORMAT_R8G8B8A8_UNORM;
	}

	CreateImage(width, height, 0, VK_IMAGE_TYPE_2D, format, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

	Buffer tmpBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, pixels, header.size - sizeof(TextureHeader), true);

	Context::CopyBufferToImage(image, width, height, tmpBuffer.GetBuffer());

	Context::TransitionImage(image, VK_FORMAT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

}
}
}