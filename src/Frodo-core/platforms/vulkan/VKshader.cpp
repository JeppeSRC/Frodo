#include <graphics/shader/shader.h>
#include <core/video/factory.h>
#include <core/video/context.h>
#include <utils/fileutils.h>
#include <core/log/log.h>

namespace fd {
namespace graphics {
namespace shader {

using namespace core;
using namespace video;
using namespace utils;
using namespace log;

Shader::Shader(const utils::String& vertexSrc, const utils::String& pixelSrc, const utils::String& geometrySrc) : vertexShader(nullptr), pixelShader(nullptr), geometryShader(nullptr) {

	char* vCode = nullptr;
	char* pCode = nullptr;

	uint_t vSize = 0;
	uint_t pSize = 0;

	if (!FileUtils::ReadFile(vertexSrc, (void**)&vCode, &vSize) || !FileUtils::ReadFile(vertexSrc, (void**)&pCode, &pSize)) {
		FD_FATAL("[Shader] Failed to load shaders");
	}

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