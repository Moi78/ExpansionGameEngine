#version 450 core
#extension GL_ARB_bindless_texture : enable
layout (location = 0) out vec3 ShadowColor;

in vec2 UVcoords;

layout(std140, binding = 15) uniform LIGHTSPACE {
	mat4 lspaceMat[10];
};

#ifndef GL_ARB_bindless_texture
	uniform sampler2D ShadowMap[10];
#else
	layout(std430, binding = 16) buffer SHADOWMAPS {
		sampler2D ShadowMap[10];
	};
#endif //GL_ARB_bindless_texture

layout(std140, binding = 17) uniform LIGHTCOUNT {
	int NbrDirLights;
};

#ifdef GL_ARB_bindless_texture
	layout(std430, binding = 8) buffer BINDLESS_PASSES {
		sampler2D passes[6];
	};
#else
	uniform sampler2D gPos;
#endif //GL_ARB_bindless_texture

float lerp(float v0, float v1, float t) {
	return (1 - t) * v0 + t * v1;
}

float PCF_Step(sampler2D map, vec3 projCoords, float currentDepth, float filterSize) {
	float bias = 0.0015;

	float shadow = 0.0;

	vec2 texelSize = 1.0 / textureSize(map, 0);
	for(float x = -filterSize; x <= filterSize; x += 1) {
		for(float y = -filterSize; y <= filterSize; y += 1) {
			float pcfDepth = texture(map, projCoords.xy + vec2(x, y) * texelSize).r;

			shadow += currentDepth - bias > pcfDepth ? 0.8 : 0.0;
		}
	}

	return shadow / (distance(-filterSize, filterSize)*distance(-filterSize, filterSize));
}

void main() {
	float finalShadow = 0.0;

	for(int i = 0; i < NbrDirLights; i++) {
		#ifdef GL_ARB_bindless_texture
			vec4 fpls = vec4(texture(passes[0], UVcoords).rgb, 1.0) * lspaceMat[i];
		#else
			vec4 fpls = vec4(texture(gPos, UVcoords).rgb, 1.0) * lspaceMat[i];
		#endif //GL_ARB_bindless_texture
		
		vec3 projCoords = fpls.xyz / fpls.w;
		projCoords = projCoords * 0.5 + 0.5;

		if(projCoords.z > 1.0) {
			ShadowColor = vec3(0.0, 0.0, 0.0);
			continue;
		}

		float closestDepth = texture(ShadowMap[i], projCoords.xy).r;
		float currentDepth = projCoords.z;

		float bias = 0.0015;
		
		finalShadow += PCF_Step(ShadowMap[i], projCoords, currentDepth, 2);
	}

	ShadowColor = vec3(clamp(1 - finalShadow, 0.0, 1.0), 0.0, 0.0);
}