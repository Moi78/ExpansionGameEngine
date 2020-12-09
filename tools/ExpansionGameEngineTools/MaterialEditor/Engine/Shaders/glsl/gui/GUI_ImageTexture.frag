#version 410
layout (location = 0) out vec4 screen;

in vec2 UVcoords;

uniform sampler2D imageTexture;
uniform float alpha;

void main() {
	vec4 tex = texture(imageTexture, UVcoords);
	tex.a = tex.a * alpha;
	screen = tex;
}