#version 450 core
#extension GL_ARB_bindless_texture : enable
layout (location = 0) out vec3 gPos;
layout (location = 1) out vec3 gNorm;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out float gSpec;
layout (location = 4) out vec3 gMetRoughAO;

in vec3 Normal;
in vec3 FragPos;
in vec2 UVcoords;

uniform sampler2D glyph;
uniform vec3 txtColor;

void main() {
	float sampled = texture(glyph, UVcoords + vec2(0.0, 1.0)).r;

	if(sampled == 0) {
		discard;
	}

	gPos = FragPos;
	gNorm = Normal;
	gAlbedo = vec4(txtColor, 1.0) * sampled;
	gSpec = 1.0;
	gMetRoughAO = vec3(0.0, 1.0, 1.0);
}