#version 410
layout (location = 0) out vec4 screen;

in vec2 UVcoords;

uniform sampler2D GlyphTex;
uniform vec3 textColor;

void main() {
	float sampled = texture(GlyphTex, UVcoords).r;
	screen = vec4(1.0, 1.0, 1.0, 1.0) * sampled;

//	Out = texture(text, UVcoords);
}