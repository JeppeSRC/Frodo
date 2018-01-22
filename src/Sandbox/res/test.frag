#version 450

#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) out vec4 Color;

layout (location = 0) in vec4 color;

layout (set = 0, binding = 0) uniform UniformBuffer {
	vec4 uColor;
};

layout (set = 0, binding = 1) uniform UniformBuffer2 {
	vec4 uColor2;
};

void main() {
	Color = color * uColor * uColor2;
}
