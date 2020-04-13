#version 410 core
layout (location = 0) out vec3 gPos;
layout (location = 1) out vec3 gNorm;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out float gSpec;

in vec3 Normal;
in vec3 FragPos;
in vec2 UVcoord;

uniform sampler2D BaseColor;
uniform sampler2D Specular;
uniform float specularExp = 256.0;

void main() {
	gPos = FragPos;

	gNorm = normalize(Normal);

	gAlbedo.rgb = texture(BaseColor, UVcoord).rgb;
	gAlbedo.a = texture(Specular, UVcoord).r;

	gSpec = specularExp;
}