#pragma once

#include <platforms/platform.h>
#include <utils/list.h>
#include <core/enums.h>
#include <graphics/texture/texture.h>
#include <graphics/texture/sampler.h>
#include <graphics/buffer/uniformbuffer.h>

namespace fd {
namespace graphics {
namespace pipeline {

typedef uint32 ShaderAccessFlags;

struct PipelineLayoutElement {
	DescriptorType type;
	uint32 binding;
	uint64 size;
	uint32 count;
	ShaderAccessFlags shaderAccess;
};

struct DescriptorBinding {
	DescriptorType type;
	uint32 set;
	uint32 binding;
	uint32 count;
	uint32 size;
	uint32 offset;
	texture::Texture* texture;
	texture::Sampler* sampler;
};

#ifdef FD_DX

#else

class PipelineLayout {
private:
	VkPipelineLayout layout;
	VkDescriptorPool descriptorPool;
	utils::List<VkDescriptorSetLayout> setLayouts;
	utils::List<VkDescriptorSet> descriptorSets;
	utils::List<VkDescriptorPoolSize> poolSizes;

	utils::List<DescriptorBinding> descriptors;

	buffer::UniformBuffer* uniformBuffer;

	utils::List<uint32> setOffsets;
public:
	PipelineLayout();
	~PipelineLayout();

	void AddSet(utils::List<PipelineLayoutElement>& elements);
	void CreateLayout();

	void UpdateUniform(uint32 set, uint32 slot, const void* const data, uint_t size, uint_t offset = 0);
	void SetTexture(uint32 set, uint32 slot, graphics::texture::Texture* texture, graphics::texture::Sampler* sampler);
	void SetTexture(uint32 set, uint32* slots, uint32 num, graphics::texture::Texture* textures, graphics::texture::Sampler* samplers);
	void SetTexture(uint32 set, uint32 slot, VkImageView view);
	void SetTexture(uint32 set, uint32* slot, uint32 num, VkImageView* view);

	inline VkPipelineLayout GetPipelineLayout() const { return layout; }
	inline VkDescriptorPool GetDescriptorPool() const { return descriptorPool; }
	inline const utils::List<VkDescriptorSetLayout>& GetSetLayouts() const { return setLayouts; }
	inline const utils::List<VkDescriptorSet>& GetDescriptorSets() const { return descriptorSets; }
	inline const utils::List<DescriptorBinding>& GetDescriptors() const { return descriptors; }

};

#endif

}
}
}