#version 450 core
#extension GL_ARB_bindless_texture : enable
layout (location = 0) out vec4 FragColor;

in vec2 UVcoords;

layout(std430, binding = 12) buffer FINAL_PASSES {
	sampler2D passes[4];
};

void main() {
	FragColor = vec4(clamp(clamp(texture(passes[0], UVcoords).rgb, 0, 1) + texture(passes[2], UVcoords).rgb, 0, 1), 1.0);
}