#version 410
in vec2 UVcoords;

out vec4 Out;

uniform sampler2D text;
uniform vec3 textColor;

void main() {
	float sampled = texture(text, UVcoords).r;
	vec4 textTex = vec4(1.0, 1.0, 1.0, sampled);
	Out = vec4(textColor, 1.0) * textTex;

//	Out = texture(text, UVcoords);
}