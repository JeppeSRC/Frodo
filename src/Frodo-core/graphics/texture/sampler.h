#pragma once

#include <platforms/platform.h>
#include <core/enums.h>
#include <core/types.h>

namespace fd {
namespace graphics {
namespace texture {

#ifdef FD_DX 

#else

class Sampler {
protected:
	VkSampler sampler;
public:
	Sampler(SamplerFilter magFilter, SamplerFilter minFilter, SamplerAddressMode addressU, SamplerAddressMode addressV, SamplerAddressMode addressW, bool anisotropy, float32 maxAnisotropy, SamplerBorderColor borderColor, bool normalizedCoordinates = true);
	~Sampler();

	inline VkSampler GetSampler() const { return sampler; }
};

#endif

}
}
}