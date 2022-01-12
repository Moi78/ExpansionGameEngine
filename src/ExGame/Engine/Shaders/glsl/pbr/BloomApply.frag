#version 450 core
#extension GL_ARB_bindless_texture : enable
layout (location = 0) out vec4 FragColor;

in vec2 UVcoords;

#ifdef GL_ARB_bindless_texture
layout(std430, binding = 12) buffer FINAL_PASSES {
	sampler2D passes[4];
};
#else
uniform sampler2D screen;
uniform sampler2D bloom;
#endif

void main() {
	#ifdef GL_ARB_bindless_texture
		FragColor = vec4(clamp(clamp(texture(passes[0], UVcoords).rgb, 0, 1) + texture(passes[2], UVcoords).rgb, 0, 1), 1.0);
	#else
		FragColor = vec4(clamp(clamp(texture(screen, UVcoords).rgb, 0, 1) + texture(bloom, UVcoords).rgb * 2, 0, 1), 1.0);
	#endif
}