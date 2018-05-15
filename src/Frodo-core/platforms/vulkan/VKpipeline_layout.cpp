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

void PipelineLayout::CreatePipelineLayout(const List<VkDescriptorSetLayout>& tmpLayouts) {
	VkPipelineLayoutCreateInfo info;

	info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.pushConstantRangeCount = 0;
	info.pPushConstantRanges = nullptr;
	info.setLayoutCount = tmpLayouts.GetSize();
	info.pSetLayouts = tmpLayouts.GetData();

	VK(vkCreatePipelineLayout(Context::GetDevice(), &info, nullptr, &pipelineLayout));
}

PipelineLayout::PipelineLayout(DescriptorSetLayout* layout) : pipelineLayout(nullptr), layouts(1){
	layouts.Push_back(layout);

	List<VkDescriptorSetLayout> tmpLayouts;

	tmpLayouts.Push_back(layout->GetSetLayout());

	CreatePipelineLayout(tmpLayouts);
}

PipelineLayout::PipelineLayout(DescriptorSetLayout** layouts, uint32 numLayouts) : pipelineLayout(nullptr), layouts(numLayouts) {
	for (uint32 i = 0; i < numLayouts; i++) {
		this->layouts.Push_back(layouts[i]);
	}

	List<VkDescriptorSetLayout> tmpLayouts(this->layouts.GetSize());

	for (uint_t i = 0; i< this->layouts.GetSize(); i++) {
		tmpLayouts.Push_back(this->layouts[i]->GetSetLayout());
	}

	CreatePipelineLayout(tmpLayouts);
}

PipelineLayout::PipelineLayout(const utils::List<DescriptorSetLayout*>& layouts) : pipelineLayout(nullptr), layouts(layouts) {

	List<VkDescriptorSetLayout> tmpLayouts(layouts.GetSize());

	for (uint_t i = 0; i< layouts.GetSize(); i++) {
		tmpLayouts.Push_back(layouts[i]->GetSetLayout());
	}


	CreatePipelineLayout(tmpLayouts);
}

PipelineLayout::~PipelineLayout() {
	
	vkDestroyPipelineLayout(Context::GetDevice(), pipelineLayout, nullptr);
}


}
}
}