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
	mat4 matrix;
};

void main() {
	gl_Position = matrix * vec4(position.x, -position.y, position.z, 1.0);	

	color = colors;
	texCoord = texCoords;
}