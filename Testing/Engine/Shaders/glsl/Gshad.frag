#version 410 core
layout (location = 0) out vec3 gPos;
layout (location = 1) out vec3 gNorm;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out float gSpec;
layout (location = 4) out float gShadow;

in vec3 Normal;
in vec3 FragPos;
in vec2 UVcoord;

in vec4 FragPosLightSpace[10];

uniform sampler2D BaseColor;
uniform sampler2D Specular;
uniform float specularExp = 256.0;

uniform sampler2D ShadowMap[10];
uniform int NbrDirLights;

vec3 norm = normalize(Normal);

float ProcessShadows(int index) {
	vec3 projCoords = FragPosLightSpace[index].xyz / FragPosLightSpace[index].w;
	projCoords = projCoords * 0.5 + 0.5;

	if(projCoords.z > 1.0) {
		return 0.0;
	}

	float closestDepth = texture(ShadowMap[index], projCoords.xy).r;
	float currentDepth = projCoords.z;

	float bias = 0.008;

	float shadow = 0.0;

	vec2 texelSize = 1.0 / textureSize(ShadowMap[index], 0);
	for(int x = -1; x <= 1; x++) {
		for(int y = -1; y <= 1; y++) {
			float pcfDepth = texture(ShadowMap[index], projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 0.5 : 0.0;
		}
	}

	return shadow / 9;
	
}

void main() {
	gPos = FragPos;

	gNorm = normalize(Normal);

	gAlbedo.rgb = texture(BaseColor, UVcoord).rgb;
	gAlbedo.a = texture(Specular, UVcoord).r;

	gSpec = specularExp;

	//Shadows Calculation
	float shadow = 0.0;
	for(int i = 0; i < NbrDirLights; i++) {
		shadow += ProcessShadows(i);
	}

	gShadow = 1.0 - clamp(shadow, 0.0, 1.0);
}