#include <graphics/shader/shader.h>
#include <core/video/factory.h>
#include <core/video/context.h>

namespace fd {
namespace graphics {
namespace shader {

using namespace core::video;

Shader::Shader(const utils::String& vertexSrc, const utils::String& pixelSrc, const utils::String& geometrySrc) : vertexShader(nullptr), pixelShader(nullptr), geometryShader(nullptr) {

	VkShaderModuleCreateInfo info;

	info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	info.flags = 0;
	info.pNext = nullptr;
	info.pCode = (const uint32*)*vertexSrc;
	info.codeSize = vertexSrc.GetLength();

	vkCreateShaderModule(Context::GetDevice(), &info, nullptr, &vertexShader);

	if (pixelSrc.GetLength() > 1) {
		info.pCode = (const uint32*)*pixelSrc;
		info.codeSize = pixelSrc.GetLength();

		vkCreateShaderModule(Context::GetDevice(), &info, nullptr, &pixelShader);
	}

	if (geometrySrc.GetLength() > 1) {
		info.pCode = (const uint32*)*geometrySrc;
		info.codeSize = geometrySrc.GetLength();

		vkCreateShaderModule(Context::GetDevice(), &info, nullptr, &pixelShader);
	}

}

Shader::~Shader() {
	vkDestroyShaderModule(Context::GetDevice(), vertexShader, nullptr);
	vkDestroyShaderModule(Context::GetDevice(), pixelShader, nullptr);
	vkDestroyShaderModule(Context::GetDevice(), geometryShader, nullptr);
}

}
}
}