#include <graphics/pipeline/layout.h>
#include <core/video/context.h>
#include <core/log/log.h>

namespace fd {
namespace graphics {
namespace pipeline {

using namespace core;
using namespace video;
using namespace utils;
using namespace log;
using namespace buffer;
using namespace texture;

PipelineLayout::PipelineLayout() : layout(nullptr), descriptorPool(nullptr) {

}

PipelineLayout::~PipelineLayout() {
	for (uint_t i = 0; i < setLayouts.GetSize(); i++) {
		vkDestroyDescriptorSetLayout(Context::GetDevice(), setLayouts[i], nullptr);
	}

	vkDestroyDescriptorPool(Context::GetDevice(), descriptorPool, nullptr);
	vkDestroyPipelineLayout(Context::GetDevice(), layout, nullptr);
}

void PipelineLayout::AddSet(List<PipelineLayoutElement>& elements) {
	VkDescriptorSetLayout setLayout = nullptr;

	if (elements.GetSize() == 0) {
		Log::Warning("[PipelineLayout] No elements!");
		return;
	}

	setOffsets.Push_back(descriptors.GetSize());

	VkDescriptorSetLayoutBinding* bindings = new VkDescriptorSetLayoutBinding[elements.GetSize()];

	for (uint_t i = 0; i < elements.GetSize(); i++) {
		VkDescriptorSetLayoutBinding& b = bindings[i];
		PipelineLayoutElement& e = elements[i];

		b.pImmutableSamplers = nullptr;
		b.binding = e.binding;
		b.stageFlags = e.shaderAccess;
		b.descriptorType = (VkDescriptorType)e.type;
		b.descriptorCount = e.count;

		VkDescriptorPoolSize size;

		size.descriptorCount = e.count;
		size.type = b.descriptorType;

		poolSizes.Push_back(size);

		DescriptorBinding binding;

		binding.type = e.type;
		binding.set = setLayouts.GetSize();
		binding.binding = b.binding;
		binding.count = e.count;
		binding.size = e.size;
		binding.offset = 0;
		binding.texture = nullptr;
		binding.sampler = nullptr;

		descriptors.Push_back(binding);
	}

	VkDescriptorSetLayoutCreateInfo info;

	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.bindingCount = elements.GetSize();
	info.pBindings = bindings;

	VK(vkCreateDescriptorSetLayout(Context::GetDevice(), &info, nullptr, &setLayout));

	setLayouts.Push_back(setLayout);

	delete[] bindings;
}

void PipelineLayout::CreateLayout() {
	VkPipelineLayoutCreateInfo info;

	info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.pushConstantRangeCount = 0;
	info.pPushConstantRanges = nullptr;
	info.setLayoutCount = setLayouts.GetSize();
	info.pSetLayouts = setLayouts.GetData();

	VK(vkCreatePipelineLayout(Context::GetDevice(), &info, nullptr, &layout));

	VkDescriptorPoolCreateInfo poolInfo;

	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.pNext = nullptr;
	poolInfo.flags = 0;
	poolInfo.maxSets = setLayouts.GetSize();
	poolInfo.poolSizeCount = poolSizes.GetSize();
	poolInfo.pPoolSizes = poolSizes.GetData();

	VK(vkCreateDescriptorPool(Context::GetDevice(), &poolInfo, nullptr, &descriptorPool));

	VkDescriptorSetAllocateInfo allocInfo;

	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.pNext = nullptr;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = setLayouts.GetSize();
	allocInfo.pSetLayouts = setLayouts.GetData();

	descriptorSets.Resize(setLayouts.GetSize());

	VK(vkAllocateDescriptorSets(Context::GetDevice(), &allocInfo, descriptorSets.GetData()));

	uint32 minOffset = Context::GetAdapter()->GetDeviceLimits().minUniformBufferOffsetAlignment;
	uint32 bufferSize = 0;

	for (uint_t i = 0; i< descriptors.GetSize(); i++) {
		DescriptorBinding& b = descriptors[i];

		if (b.type == DescriptorType::Uniform) {
			b.offset = bufferSize;
			bufferSize += b.size >= minOffset ? b.size : minOffset;
		}
	}

	uniformBuffer = new UniformBuffer(nullptr, bufferSize);

	List<VkWriteDescriptorSet> winfo;

	VkDescriptorBufferInfo binfo[1024];

	for (uint_t i = 0; i < descriptors.GetSize(); i++) {
		DescriptorBinding& b = descriptors[i];

		if (b.type == DescriptorType::Uniform) {
			VkWriteDescriptorSet info;

			info.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			info.pNext = nullptr;
			info.dstSet = descriptorSets[b.set];
			info.dstBinding = b.binding;
			info.descriptorType = (VkDescriptorType)b.type;
			info.descriptorCount = 1;
			info.dstArrayElement = 0;
			info.pTexelBufferView = nullptr;
			info.pImageInfo = nullptr;
			info.pBufferInfo = binfo + i;

			binfo[i].buffer = uniformBuffer->GetBuffer();
			binfo[i].offset = b.offset;
			binfo[i].range = b.size;

			winfo.Push_back(info);
		}
	}

	vkUpdateDescriptorSets(Context::GetDevice(), winfo.GetSize(), winfo.GetData(), 0, nullptr);
}

void PipelineLayout::UpdateUniform(uint32 set, uint32 binding, const void* const data, uint_t size, uint_t offset) {
	DescriptorBinding& b = descriptors[setOffsets[set] + binding];

	if (b.type != DescriptorType::Uniform) {
		Log::Fatal("[PipelineLayout] binding %u in set %u is not a uniform!", binding, set);
		return;
	}

	void* bufferData = uniformBuffer->Map(b.offset + offset, size);
	FD_ASSERT(bufferData != nullptr);
	memcpy(bufferData, data, size);
	uniformBuffer->Unmap();
}

void PipelineLayout::SetTexture(uint32 set, uint32 slots, Texture* texture, Sampler* sampler) {
	SetTexture(set, &slots, 1, texture, sampler);
}

void PipelineLayout::SetTexture(uint32 set, uint32* slots, uint32 num, Texture* textures, Sampler* samplers) {
	VkWriteDescriptorSet* winfo = new VkWriteDescriptorSet[num];

	VkDescriptorImageInfo* iinfo = new VkDescriptorImageInfo[num];

	for (uint32 i = 0; i < num; i++) {
		uint32 slot = slots[i];
		DescriptorBinding& binding = descriptors[setOffsets[set] + slot];

		if (binding.type != DescriptorType::TextureSampler || binding.type != DescriptorType::InputAttachment) {
			Log::Fatal("[PipelineLayout] binding %u in set %u is not a \"TextureSampler\" or \"InputAttachment\"", slot, set);
			break;
		}

		winfo[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		winfo[i].pNext = nullptr;
		winfo[i].descriptorType = (VkDescriptorType)binding.type;
		winfo[i].dstSet = descriptorSets[set];
		winfo[i].dstBinding = slot;
		winfo[i].dstArrayElement = 0;
		winfo[i].descriptorCount = 1;
		winfo[i].pBufferInfo = nullptr;
		winfo[i].pTexelBufferView = nullptr;

		winfo[i].pImageInfo = iinfo + i;

		iinfo[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		iinfo[i].imageView = textures[i].GetImageView();
		binding.texture = textures + i;

		if (binding.type != DescriptorType::TextureSampler) {
			iinfo[i].sampler = samplers[i].GetSampler();
			binding.sampler = samplers + i;
		}
	}

	vkUpdateDescriptorSets(Context::GetDevice(), num, winfo, 0, 0);
}

void PipelineLayout::SetTexture(uint32 set, uint32 slot, VkImageView view) {
	SetTexture(set, &slot, 1, &view);
}

void PipelineLayout::SetTexture(uint32 set, uint32* slots, uint32 num, VkImageView* views) {
	VkWriteDescriptorSet* winfo = new VkWriteDescriptorSet[num];

	VkDescriptorImageInfo* iinfo = new VkDescriptorImageInfo[num];

	for (uint32 i = 0; i < num; i++) {
		uint32 slot = slots[i];
		DescriptorBinding& binding = descriptors[setOffsets[set] + slot];

		FD_ASSERT(binding.type == DescriptorType::TextureSampler);

		winfo[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		winfo[i].pNext = nullptr;
		winfo[i].descriptorType = (VkDescriptorType)binding.type;
		winfo[i].dstSet = descriptorSets[set];
		winfo[i].dstBinding = slot;
		winfo[i].dstArrayElement = 0;
		winfo[i].descriptorCount = 1;
		winfo[i].pBufferInfo = nullptr;
		winfo[i].pTexelBufferView = nullptr;

		winfo[i].pImageInfo = iinfo + i;

		iinfo[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		iinfo[i].imageView = views[i];
		iinfo[i].sampler = nullptr;

		binding.texture = nullptr;
		binding.sampler = nullptr;
	}

	vkUpdateDescriptorSets(Context::GetDevice(), num, winfo, 0, 0);
}

}
}
}