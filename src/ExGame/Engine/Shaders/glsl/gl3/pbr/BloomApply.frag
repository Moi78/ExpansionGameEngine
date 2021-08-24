#version 330 core
layout (location = 0) out vec4 FragColor;

in vec2 UVcoords;

uniform sampler2D screen;
uniform sampler2D bloom;

void main() {
	FragColor = vec4(clamp(clamp(texture(screen, UVcoords).rgb, 0, 1) + texture(bloom, UVcoords).rgb * 2, 0, 1), 1.0);
}