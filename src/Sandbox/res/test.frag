#version 450

#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) out vec4 Color;

layout (location = 0) in vec4 color;
layout (location = 1) in vec2 texCoord;

layout (binding = 1) uniform sampler2D tex;

void main() {
	Color = color * texture(tex, texCoord);
}
