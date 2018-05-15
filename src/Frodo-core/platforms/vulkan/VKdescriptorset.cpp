#include <graphics/pipeline/layout.h>
#include <core/video/context.h>
#include <core/log/log.h>
#include <graphics/texture/texture.h>
#include <graphics/texture/sampler.h>

namespace fd {
namespace graphics {
namespace pipeline {

using namespace core;
using namespace video;
using namespace utils;
using namespace buffer;
using namespace log;
using namespace texture;

DescriptorBinding& DescriptorSet::GetBinding(uint32 binding) {
	uint_t size = bindings.GetSize();

	for (uint_t i = 0; i < size; i++) {
		DescriptorBinding& b = bindings[i];

		if (b.binding == binding) {
			return b;
		}
	}

	Log::Warning("[DescriptorSet] Binding %u isn't a thing!", binding);

	FD_ASSERT(false);

	return DescriptorBinding();
}

DescriptorSet::DescriptorSet(VkDescriptorSet set, const List<DescriptorBinding>& binding) : set(set), bindings(binding) {
	List<VkWriteDescriptorSet> winfos;
	
	VkDescriptorBufferInfo binfo[1024];

	VkWriteDescriptorSet winfo;

	winfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	winfo.pNext = nullptr;
	winfo.dstSet = set;
	winfo.dstArrayElement = 0;
	winfo.pBufferInfo = nullptr;
	winfo.pImageInfo = nullptr;
	winfo.pTexelBufferView = nullptr;

	for (uint_t i = 0; i < bindings.GetSize(); i++) {
		const DescriptorBinding& b = bindings[i];
		
		winfo.descriptorType = (VkDescriptorType)b.type;
		winfo.dstBinding = b.binding;
		winfo.descriptorCount = b.count;
		
		if (b.type == DescriptorType::Uniform) {
			winfo.pBufferInfo = binfo + i;

			binfo[i].buffer = b.buffer->GetBuffer();
			binfo[i].offset = b.offset;
			binfo[i].range = b.size;
			
			winfos.Push_back(winfo);
		}
	}

	vkUpdateDescriptorSets(Context::GetDevice(), winfos.GetSize(), winfos.GetData(), 0, nullptr);
}

void DescriptorSet::Update() {
	List<VkWriteDescriptorSet> winfos;

	VkDescriptorBufferInfo binfo[1024];

	VkWriteDescriptorSet winfo;

	winfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	winfo.pNext = nullptr;
	winfo.dstSet = set;
	winfo.dstArrayElement = 0;
	winfo.pBufferInfo = nullptr;
	winfo.pImageInfo = nullptr;
	winfo.pTexelBufferView = nullptr;

	for (uint_t i = 0; i < bindings.GetSize(); i++) {
		const DescriptorBinding& b = bindings[i];

		winfo.descriptorType = (VkDescriptorType)b.type;
		winfo.dstBinding = b.binding;
		winfo.descriptorCount = b.count;

		if (b.type == DescriptorType::Uniform) {
			winfo.pBufferInfo = binfo + i;

			binfo[i].buffer = b.buffer->GetBuffer();
			binfo[i].offset = b.offset;
			binfo[i].range = b.size;
			
			winfos.Push_back(winfo);
		}

	}

	vkUpdateDescriptorSets(Context::GetDevice(), winfos.GetSize(), winfos.GetData(), 0, nullptr);
}

void DescriptorSet::UpdateUniform(uint32 binding, const void* const data, uint_t size, uint_t offset) {
	const DescriptorBinding& b = GetBinding(binding);

	if (b.type != DescriptorType::Uniform) {
		Log::Fatal("[PipelineLayout] binding %u is not a uniform!", binding);
		return;
	}

	void* bufferData = b.buffer->Map(b.offset + offset, size);
	FD_ASSERT(bufferData != nullptr);
	memcpy(bufferData, data, size);
	b.buffer->Unmap();
}

void DescriptorSet::SetTexture(uint32 binding, Texture* texture, Sampler* sampler) {
	SetTexture(&binding, 1, texture, sampler);
}

void DescriptorSet::SetTexture(uint32* bindings, uint32 num, Texture* textures, Sampler* samplers) {
	VkWriteDescriptorSet* winfo = new VkWriteDescriptorSet[num];

	VkDescriptorImageInfo* iinfo = new VkDescriptorImageInfo[num];

	for (uint32 i = 0; i < num; i++) {
		uint32 binding = bindings[i];
		DescriptorBinding& b = GetBinding(binding);

		if (b.type != DescriptorType::TextureSampler && b.type != DescriptorType::InputAttachment) {
			Log::Fatal("[PipelineLayout] binding %u is not a \"TextureSampler\" or \"InputAttachment\"", binding);
			break;
		}

		winfo[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		winfo[i].pNext = nullptr;
		winfo[i].descriptorType = (VkDescriptorType)b.type;
		winfo[i].dstSet = set;
		winfo[i].dstBinding = b.binding;
		winfo[i].dstArrayElement = 0;
		winfo[i].descriptorCount = 1;
		winfo[i].pBufferInfo = nullptr;
		winfo[i].pTexelBufferView = nullptr;

		winfo[i].pImageInfo = iinfo + i;

		iinfo[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		iinfo[i].imageView = textures[i].GetImageView();
		b.texture = textures + i;

		if (b.type == DescriptorType::TextureSampler) {
			iinfo[i].sampler = samplers[i].GetSampler();
			b.sampler = samplers + i;
		}
	}

	vkUpdateDescriptorSets(Context::GetDevice(), num, winfo, 0, 0);

	delete[] winfo;
	delete[] iinfo;
}

void DescriptorSet::SetTexture(uint32 binding, VkImageView view) {
	SetTexture(&binding, 1, &view);
}

void DescriptorSet::SetTexture(uint32* bindings, uint32 num, VkImageView* views) {
	VkWriteDescriptorSet* winfo = new VkWriteDescriptorSet[num];

	VkDescriptorImageInfo* iinfo = new VkDescriptorImageInfo[num];

	for (uint32 i = 0; i < num; i++) {
		uint32 binding = bindings[i];
		DescriptorBinding& b = this->bindings[i];

		if (b.type != DescriptorType::TextureSampler && b.type != DescriptorType::InputAttachment) {
			Log::Fatal("[PipelineLayout] binding %u is not a \"TextureSampler\" or \"InputAttachment\"", binding);
			break;
		}

		winfo[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		winfo[i].pNext = nullptr;
		winfo[i].descriptorType = (VkDescriptorType)b.type;
		winfo[i].dstSet = set;
		winfo[i].dstBinding = b.binding;
		winfo[i].dstArrayElement = 0;
		winfo[i].descriptorCount = 1;
		winfo[i].pBufferInfo = nullptr;
		winfo[i].pTexelBufferView = nullptr;

		winfo[i].pImageInfo = iinfo + i;

		iinfo[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		iinfo[i].imageView = views[i];
		iinfo[i].sampler = nullptr;

		b.texture = nullptr;
		b.sampler = nullptr;
	}

	vkUpdateDescriptorSets(Context::GetDevice(), num, winfo, 0, 0);

	delete[] winfo;
	delete[] iinfo;
}



}
}
}