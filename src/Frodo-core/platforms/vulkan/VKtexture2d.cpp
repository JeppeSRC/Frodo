#include <graphics/texture/texture.h>
#include <core/video/context.h>

namespace fd {
namespace graphics {
namespace texture {

using namespace FD;
using namespace utils;
using namespace buffer;
using namespace core::video;

Texture2D::Texture2D(uint32 width, uint32 height, VkFormat format, VkImageUsageFlags usage, VkImageLayout layout) : Texture(width, height), format(format) {

	CreateImage(width, height, 0, VK_IMAGE_TYPE_2D, format, usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, layout);

	VkImageViewCreateInfo vinfo;

	VkImageAspectFlags aspect = 0;

	if (usage & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) {
		aspect = VK_IMAGE_ASPECT_COLOR_BIT;
	} else if (usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT) {
		aspect = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
	}

	vinfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	vinfo.pNext = nullptr;
	vinfo.flags = 0;
	vinfo.image = image;
	vinfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	vinfo.format = format;
	vinfo.components.r = VK_COMPONENT_SWIZZLE_R;
	vinfo.components.g = VK_COMPONENT_SWIZZLE_G;
	vinfo.components.b = VK_COMPONENT_SWIZZLE_B;
	vinfo.components.a = VK_COMPONENT_SWIZZLE_A;
	vinfo.subresourceRange.aspectMask = aspect;
	vinfo.subresourceRange.baseArrayLayer = 0;
	vinfo.subresourceRange.baseMipLevel = 0;
	vinfo.subresourceRange.layerCount = 1;
	vinfo.subresourceRange.levelCount = 1;

	VK(vkCreateImageView(Context::GetDevice(), &vinfo, nullptr, &imageView));

}
	
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

	VkImageViewCreateInfo vinfo;

	vinfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	vinfo.pNext = nullptr;
	vinfo.flags = 0;
	vinfo.image = image;
	vinfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	vinfo.format = format;
	vinfo.components.r = VK_COMPONENT_SWIZZLE_R;
	vinfo.components.g = VK_COMPONENT_SWIZZLE_G;
	vinfo.components.b = VK_COMPONENT_SWIZZLE_B;
	vinfo.components.a = VK_COMPONENT_SWIZZLE_A;
	vinfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	vinfo.subresourceRange.baseArrayLayer = 0;
	vinfo.subresourceRange.baseMipLevel = 0;
	vinfo.subresourceRange.layerCount = 1;
	vinfo.subresourceRange.levelCount = 1;

	VK(vkCreateImageView(Context::GetDevice(), &vinfo, nullptr, &imageView));


}

}
}
}