#include <graphics/pipeline/layout.h>
#include <core/video/context.h>
#include <core/log/log.h>

namespace fd {
namespace graphics {
namespace pipeline {

using namespace core;
using namespace video;
using namespace utils;
using namespace buffer;
using namespace log;

DescriptorSetLayout::DescriptorSetLayout(const utils::List<DescriptorSetBinding>& setBindings, uint32 numSets) : numSets(numSets), bindings(setBindings), uniformBuffer(nullptr), uniformBufferOffset(0) {

	VkDescriptorSetLayoutBinding* bnd = new VkDescriptorSetLayoutBinding[bindings.GetSize()];

	for (uint_t i = 0; i < setBindings.GetSize(); i++) {
		const DescriptorSetBinding& d = setBindings[i];
		VkDescriptorSetLayoutBinding& b = bnd[i];

		b.pImmutableSamplers = nullptr;
		b.binding = d.binding;
		b.descriptorType = (VkDescriptorType)d.type;
		b.descriptorCount = d.count;
		b.stageFlags = (VkShaderStageFlags)d.shaderAccess;

		VkDescriptorPoolSize size;

		size.type = b.descriptorType;
		size.descriptorCount = b.descriptorCount * numSets;

		poolSizes.Push_back(size);
	}

	VkDescriptorSetLayoutCreateInfo info;

	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.bindingCount = bindings.GetSize();
	info.pBindings = bnd;

	VK(vkCreateDescriptorSetLayout(Context::GetDevice(), &info, nullptr, &setLayout));

	VkDescriptorPoolCreateInfo pinfo;

	pinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pinfo.pNext = nullptr;
	pinfo.flags = 0;
	pinfo.maxSets = numSets;
	pinfo.poolSizeCount = poolSizes.GetSize();
	pinfo.pPoolSizes = poolSizes.GetData();

	VkDescriptorPool pool;

	VK(vkCreateDescriptorPool(Context::GetDevice(), &pinfo, nullptr, &pool));

	pools.Push_back(pool);
}

DescriptorSetLayout::~DescriptorSetLayout() {
	for (uint_t i = 0; i < pools.GetSize(); i++) {
		vkDestroyDescriptorPool(Context::GetDevice(), pools[i], nullptr);
	}

	for (uint_t i = 0; i < descriptorSets.GetSize(); i++) {
		delete descriptorSets[i];
	}

	vkDestroyDescriptorSetLayout(Context::GetDevice(), setLayout, nullptr);

	delete uniformBuffer;

	
}

DescriptorSet* DescriptorSetLayout::AllocateDescriptorSet() {
	VkDescriptorSetAllocateInfo info;

	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	info.pNext = nullptr;
	info.descriptorPool = pools[pools.GetSize()-1];
	info.descriptorSetCount = 1;
	info.pSetLayouts = &setLayout;

	VkDescriptorSet tmpSet;

	if (descriptorSets.GetSize() >= numSets) {
		Log::Fatal("[DescriptorSetLayout] Max descriptorset allocation reached!");
		return nullptr;
	}

	VK(vkAllocateDescriptorSets(Context::GetDevice(), &info, &tmpSet));

	List<DescriptorBinding> setBindings;

	setBindings.Resize(bindings.GetSize());

	uint32 minUniformOffset = Context::GetAdapter()->GetDeviceLimits().minUniformBufferOffsetAlignment;

	for (uint_t i = 0; i < setBindings.GetSize(); i++) {
		const DescriptorSetBinding& e = bindings[i];
		DescriptorBinding& tmpBinding = setBindings[i];

		if (e.type == DescriptorType::Uniform) {
			tmpBinding.offset = uniformBufferOffset;
			uniformBufferOffset += e.size > minUniformOffset ? e.size : minUniformOffset;
		}
	}

	for (uint_t i = 0; i < bindings.GetSize(); i++) {
		const DescriptorSetBinding& e = bindings[i];

		DescriptorBinding& tmpBinding = setBindings[i];

		tmpBinding.type = e.type;
		tmpBinding.binding = e.binding;
		tmpBinding.count = e.count;

		if (e.type == DescriptorType::Uniform) {
			if (uniformBuffer == nullptr) {
				uniformBuffer = new UniformBuffer(nullptr, uniformBufferOffset * numSets);
			}

			tmpBinding.size = e.size;
			tmpBinding.buffer = uniformBuffer;
			tmpBinding.texture = nullptr;
			tmpBinding.sampler = nullptr;
		}

	}

	DescriptorSet* tmp = new DescriptorSet(tmpSet, setBindings);

	descriptorSets.Push_back(tmp);

	return tmp;
}


}
}
}