#version 410 core

//In
in vec2 UVcoords;
in vec3 Normal;
in vec4 FragPosLightSpace;
in vec4 FragPos;

//Out
out vec3 FragColor;

uniform sampler2D ShadowMap;

uniform mat4 lspaceMat;
uniform vec3 DirLightDir;

vec3 norm = normalize(Normal);

float ProcessShadow() {
	vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;

	if(projCoords.z > 1.0) {
		return 0.0;
	}

	float closestDepth = texture(ShadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;

	float bias = max(0.05 * (1.0 - dot(norm, DirLightDir)), 0.005);

	float shadow = 1.0;
	if(closestDepth < currentDepth - bias) {
		return 0.5;
	}
}

void main() {
	FragColor = (1 - ProcessShadow()) * vec3(1.0, 1.0, 1.0);
}