#version 450

#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 colors;

out gl_PerVertex {
	vec4 gl_Position;
};

layout (location = 0) out vec4 color;

void main() {
	gl_Position = vec4(position, 1.0);	

	color = colors;
}