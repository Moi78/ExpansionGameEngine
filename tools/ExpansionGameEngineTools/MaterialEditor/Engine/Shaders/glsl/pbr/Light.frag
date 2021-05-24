#version 450 core
#extension GL_ARB_bindless_texture : enable
layout (location = 0) out vec4 LightPass;

in vec2 UVcoords;

//uniform sampler2D GUIscreen;

//Passes
uniform sampler2D ShadowPass;
uniform sampler2D ssao;

#ifndef GL_ARB_bindless_texture
	uniform sampler2D gPos;
	uniform sampler2D gNormal;
	uniform sampler2D gAlbedo;
	uniform sampler2D gSpec;
	uniform sampler2D gMetRoughAO;
	uniform sampler2D gEmissive;
#else
	layout(std430, binding = 8) buffer BINDLESS_PASSES {
		sampler2D passes[6];
	};
#endif //ARB_bindless_texture
//Ambient
layout(std140, binding = 2) uniform AMBIENT {
	vec3 AmbientColor;
	float AmbientStrength;
};

//Point Light
const int MAX_POINT_LIGHTS = 243;

struct PointLight {
	vec3 LightPos;
	vec3 LightColor;
	float LightBrightness;
	float LightRadius;
};

layout(std140, binding = 3) uniform PointLightData {
	int nbrPointLight;
	PointLight plights[MAX_POINT_LIGHTS];
};

//Dir Light
struct DirLight {
	vec3 Dir;
	vec3 Color;
	float brightness;
};

layout(std140, binding = 4) uniform DirLightData {
	int nbrDirLight;
	DirLight dlights[10];
};

layout(std140, binding = 5) uniform CamData {
	vec3 CamPos;
};

uniform bool ftr_lighting = true;
uniform bool ftr_specular = true;
uniform bool ftr_ambient = true;

float PI = 3.14159265359;

#ifdef GL_ARB_bindless_texture
	vec3 norm = normalize(texture(passes[1], UVcoords).rgb);
	vec3 FragPos = texture(passes[0], UVcoords).rgb;

	vec3 Diffuse = texture(passes[2], UVcoords).rgb;
	float Specular = texture(passes[2], UVcoords).a;
	float SpecularExp = texture(passes[3], UVcoords).r;

	vec4 metrao = texture(passes[4], UVcoords);
#else 
	vec3 norm = normalize(texture(gNorm, UVcoords).rgb);
	vec3 FragPos = texture(gPos, UVcoords).rgb;

	vec3 Diffuse = texture(gAlbedo, UVcoords).rgb;
	float Specular = texture(gAlbedo, UVcoords).a;
	float SpecularExp = texture(gSpec, UVcoords).r;
#endif //GL_ARB_bindless_texture

float Rness = metrao.g;
float Metllc = metrao.r;
float AO = metrao.b;

vec3 viewDir = normalize((CamPos - FragPos));

vec3 F0 = mix(vec3(0.04), Diffuse, Metllc);

vec3 FresnelSchlick(float cosTheta, vec3 F0) {
	return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness) {
	float a = roughness*roughness;
	float a2 = a*a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH*NdotH;

	float num = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return num / denom;
}

float GeomSchlickGGX(float NdotV, float roughness) {
	float r = (roughness + 1.0);
	float k = (r*r) / 8.0;

	float num = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return num / denom;
}

float GeomSmith(vec3 N, vec3 V, vec3 L, float roughness) {
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeomSchlickGGX(NdotV, roughness);
	float ggx1 = GeomSchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

vec3 CalcDirLight(int index) {
	float dist = 100000.0;

	vec3 ColPow = dlights[index].Color * dlights[index].brightness;

	vec3 L = normalize(vec3(-dlights[index].Dir));
	vec3 H = normalize(viewDir + L);

	vec3 radiance = ColPow;

	float NDF = DistributionGGX(norm, H, Rness);
	float G = GeomSmith(norm, viewDir, L, Rness);
	vec3 F = FresnelSchlick(max(dot(H, viewDir), 0.0), F0);

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - Metllc;

	vec3 numerator = NDF * G * F;
	float denominator = 4.0 * max(dot(norm, viewDir), 0.0) * max(dot(norm, L), 0.0) + 0.001;
	vec3 specular = numerator / denominator;

	float NdotL = max(dot(norm, L), 0.0);

	return (kD * Diffuse / PI + specular) * radiance * NdotL;
}

vec3 CalcPointLight(int lightIndex) {
	float dist = length(plights[lightIndex].LightPos - FragPos);

	if(dist < plights[lightIndex].LightRadius) {
		vec3 ColPow = plights[lightIndex].LightColor * plights[lightIndex].LightBrightness;

		vec3 L = normalize(plights[lightIndex].LightPos - FragPos);
		vec3 H = normalize(viewDir + L);

		float attenuation = 1.0 / ((dist * dist));
		vec3 radiance = ColPow * attenuation;

		float NDF = DistributionGGX(norm, H, Rness);
		float G = GeomSmith(norm, viewDir, L, Rness);
		vec3 F = FresnelSchlick(max(dot(H, viewDir), 0.0), F0);

		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - Metllc;

		vec3 numerator = NDF * G * F;
		float denominator = 4.0 * max(dot(norm, viewDir), 0.0) * max(dot(norm, L), 0.0) + 0.001;
		vec3 specular = numerator / denominator;

		float NdotL = max(dot(norm, L), 0.0);
		return (kD * Diffuse / PI + specular) * radiance * NdotL;
	} else {
		return vec3(0.0);
	}
}

void main() {
	vec3 diffSpec = vec3(0.0);

	float shadow = texture(ShadowPass, UVcoords).r;

	for(int i = 0; i < nbrDirLight; i++) {
		diffSpec += max(CalcDirLight(i), 0.0) * shadow;
	}

	for(int i = 0; i < nbrPointLight; i++) {
		diffSpec += max(CalcPointLight(i), 0.0);
	}

	float SSAO = texture(ssao, UVcoords).r;
	vec3 ambient = (AmbientColor * AmbientStrength) * Diffuse * AO;
	vec3 result = (diffSpec + ambient) * SSAO;

	//Gamma
	result = result / (result + vec3(1.0));
	result = pow(result, vec3(1.0 / 2.2));

	//Emissive color
	result += texture(passes[5], UVcoords).rgb;

	LightPass = vec4(result, 1.0);
}