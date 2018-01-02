#pragma once

#include <platforms/platform.h>
#include <utils/string.h>

namespace fd {
namespace graphics {
namespace shader {

#ifdef FD_DX 

#else

class Shader {
private:
	VkShaderModule vertexShader;
	VkShaderModule pixelShader;
	VkShaderModule geometryShader;

public:
	Shader(const utils::String& vertexSrc, const utils::String& pixelSrc, const utils::String& geometrySrc);
	~Shader();

	inline VkShaderModule GetVertexShader() const { return vertexShader; }
	inline VkShaderModule GetPixelShader() const { return pixelShader; }
	inline VkShaderModule GetGeometryShader() const { return geometryShader; }
};

#endif

} } }