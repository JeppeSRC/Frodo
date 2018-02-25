#include <graphics/texture/sampler.h>
#include <core/video/context.h>

namespace fd {
namespace graphics {
namespace texture {

using namespace core::video;

Sampler::Sampler(SamplerFilter magFilter, SamplerFilter minFilter, SamplerAddressMode addressU, SamplerAddressMode addressV, SamplerAddressMode addressW, bool anisotropy, float32 maxAnisotropy, SamplerBorderColor borderColor, bool normalizedCoordinates) {
	VkSamplerCreateInfo info;

	info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	info.pNext = nullptr;
	info.magFilter = (VkFilter)magFilter;
	info.minFilter = (VkFilter)minFilter;
	info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	info.addressModeU = (VkSamplerAddressMode)addressU;
	info.addressModeV = (VkSamplerAddressMode)addressV;
	info.addressModeW = (VkSamplerAddressMode)addressW;
	info.mipLodBias = 0.0f;
	info.anisotropyEnable = anisotropy;
	info.maxAnisotropy = maxAnisotropy;
	info.compareEnable = false;
	info.compareOp = VK_COMPARE_OP_ALWAYS;
	info.minLod = 0.0f;
	info.maxLod = 0.0f;
	info.borderColor = (VkBorderColor)borderColor;
	info.unnormalizedCoordinates = !normalizedCoordinates;

	VK(vkCreateSampler(Context::GetDevice(), &info, nullptr, &sampler));
}

Sampler::~Sampler() {
	vkDestroySampler(Context::GetDevice(), sampler, nullptr);
}

}
}
}