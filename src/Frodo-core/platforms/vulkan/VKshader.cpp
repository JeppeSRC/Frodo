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
	info.pCode = (const uint32*)vCode;
	info.codeSize = vSize;

	VkResult res = vkCreateShaderModule(Context::GetDevice(), &info, nullptr, &vertexShader);

	if (res != VK_SUCCESS) {
		FD_FATAL("[Shader] Failed to load vertex shader module");
	}

	if (pixelSrc.GetLength() > 1) {
		info.pCode = (const uint32*)pCode;
		info.codeSize = pSize;

		res = vkCreateShaderModule(Context::GetDevice(), &info, nullptr, &pixelShader);

		if (res != VK_SUCCESS) {
			FD_FATAL("[Shader] Failed to load pixelööö shader module");
		}
	}

	if (geometrySrc.GetLength() > 1) {
		info.pCode = (const uint32*)*geometrySrc;
		info.codeSize = geometrySrc.GetLength();

		res = vkCreateShaderModule(Context::GetDevice(), &info, nullptr, &pixelShader);

		if (res != VK_SUCCESS) {
			FD_FATAL("[Shader] Failed to load geometry vertex shader module");
		}
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