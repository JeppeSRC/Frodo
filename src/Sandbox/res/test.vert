#version 450

#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 colors;
layout (location = 2) in vec2 texCoords;

out gl_PerVertex {
	vec4 gl_Position;
};

layout (location = 0) out vec4 color;
layout (location = 1) out vec2 texCoord;

layout (binding = 0) uniform UniformBuffer {
	mat4 projection;
	mat4 model;
};

void main() {
	gl_Position = projection * model * vec4(position.x, -position.y, position.z, 1.0);	

	color = colors;
	texCoord = (model * vec4(texCoords - vec2(0.5, 0.5), 0, 0.5)).xy + vec2(0.5f, 0.5f);
	//texCoord = texCoords;
}