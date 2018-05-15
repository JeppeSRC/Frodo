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

struct DescriptorSetBinding {
	DescriptorType type;
	uint32 binding;
	uint64 size;
	uint32 count;
	ShaderAccessFlags shaderAccess;
};

struct DescriptorBinding {
	DescriptorType type;
	uint32 binding;
	uint32 count;
	uint32 size;
	uint32 offset;
	buffer::UniformBuffer* buffer;
	texture::Texture* texture;
	texture::Sampler* sampler;
};

class DescriptorSet {
private:
	VkDescriptorSet set;
	utils::List<DescriptorBinding> bindings;

	DescriptorBinding& GetBinding(uint32 binding);
public:
	DescriptorSet(VkDescriptorSet set, const utils::List<DescriptorBinding>& binding);

	void Update();

	void UpdateUniform(uint32 binding, const void* const data, uint_t size, uint_t offset = 0);
	void SetTexture(uint32 binding, graphics::texture::Texture* texture, graphics::texture::Sampler* sampler);
	void SetTexture(uint32* bindings, uint32 num, graphics::texture::Texture* textures, graphics::texture::Sampler* samplers);
	void SetTexture(uint32 binding, VkImageView view);
	void SetTexture(uint32* bindings, uint32 num, VkImageView* view);

	inline const VkDescriptorSet& GetDescriptorSet() const { return set; }
};

class DescriptorSetLayout {
private:
	VkDescriptorSetLayout setLayout;

	uint32 numSets;

	utils::List<VkDescriptorPoolSize> poolSizes;
	utils::List<VkDescriptorPool> pools;

	utils::List<DescriptorSetBinding> bindings;
	utils::List<DescriptorSet*> descriptorSets;

	buffer::UniformBuffer* uniformBuffer;
	uint32 uniformBufferOffset;

public:
	DescriptorSetLayout(const utils::List<DescriptorSetBinding>& setBindings, uint32 numSets);
	~DescriptorSetLayout();

	DescriptorSet* AllocateDescriptorSet();

	inline const VkDescriptorSetLayout& GetSetLayout() const { return setLayout; }
};

class PipelineLayout {
private:
	VkPipelineLayout pipelineLayout;

	utils::List<DescriptorSetLayout*> layouts;
	
	void CreatePipelineLayout(const utils::List<VkDescriptorSetLayout>& layouts);
public:
	PipelineLayout(DescriptorSetLayout* layout);
	PipelineLayout(DescriptorSetLayout** layouts, uint32 numLayouts);
	PipelineLayout(const utils::List<DescriptorSetLayout*>& layouts);
	~PipelineLayout();

	inline VkPipelineLayout GetPipelineLayout() const { return pipelineLayout; }
	inline const utils::List<DescriptorSetLayout*>& GetSetLayouts() const { return layouts; }
};

}
}
}