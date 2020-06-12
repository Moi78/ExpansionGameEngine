#version 410 core
layout (location = 0) out vec3 gPos;
layout (location = 1) out vec3 gNorm;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out float gSpec;
layout (location = 4) out float gShadow;

in vec3 Normal;
in vec3 Tan;
in vec3 Bitan;

in vec3 FragPos;
in vec2 UVcoord;

in vec4 FragPosLightSpace[10];

uniform sampler2D BaseColor;
uniform sampler2D Specular;
uniform float specularExp = 256.0;

uniform sampler2D NormalMap;
uniform bool NormalEnabled = false;

uniform sampler2D ShadowMap[10];
uniform int NbrDirLights;

vec3 norm = normalize(Normal);

vec3 GetNormFromMap(sampler2D NMap, vec3 n, vec3 FPos, vec2 UV) {
	vec3 tanNorm = texture(NMap, UVcoord).rgb * 2.0 - 1.0;

	vec3 Q1 = dFdx(FPos);
	vec3 Q2 = dFdy(FPos);
	vec2 st1 = dFdx(UV);
	vec2 st2 = dFdy(UV);

	vec3 T = normalize(Q1 * st2.t - Q2 * st1.t);
	vec3 B = -normalize(cross(n, T));
	mat3 TBN = mat3(T, B, n);

	return normalize(TBN * tanNorm);
}

float ProcessShadows(sampler2D SMap, vec4 FPLS) {
	vec3 projCoords = FPLS.xyz / FPLS.w;
	projCoords = projCoords * 0.5 + 0.5;

	if(projCoords.z > 1.0) {
		return 0.0;
	}

	float closestDepth = texture(SMap, projCoords.xy).r;
	float currentDepth = projCoords.z;

	float bias = 0.008;

	float shadow = 0.0;

	vec2 texelSize = 1.0 / textureSize(SMap, 0);
	for(int x = -1; x <= 1; x++) {
		for(int y = -1; y <= 1; y++) {
			float pcfDepth = texture(SMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}

	return shadow / 9;
	
}

void main() {
	gPos = FragPos;

	if(NormalEnabled) {
		gNorm = GetNormFromMap(NormalMap, norm, FragPos, UVcoord);
	} else {
		gNorm = norm;
	}

	gAlbedo.rgb = texture(BaseColor, UVcoord).rgb;
	gAlbedo.a = texture(Specular, UVcoord).r;

	gSpec = specularExp;

	//Shadows Calculation
	float shadow = 0.0;
	for(int i = 0; i < NbrDirLights; i++) {
		shadow += ProcessShadows(ShadowMap[i], FragPosLightSpace[i]);
	}

	gShadow = 1.0 - clamp(shadow, 0.0, 1.0);
}