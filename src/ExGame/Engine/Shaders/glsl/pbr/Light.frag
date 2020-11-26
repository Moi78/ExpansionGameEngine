#version 410 core
layout (location = 0) out vec4 LightPass;

in vec2 UVcoords;

//uniform sampler2D GUIscreen;

//Passes
uniform sampler2D ShadowPass;

uniform sampler2D gPos;
uniform sampler2D gNormal;

uniform sampler2D gAlbedo;
uniform sampler2D gSpec;
uniform sampler2D gMetRoughAO;

//Ambient
uniform float AmbientStrength;
uniform vec3 AmbientColor;

//Point Light
const int MAX_POINT_LIGHTS = 243;
uniform vec3 LightPos[MAX_POINT_LIGHTS];
uniform float LightBrightness[MAX_POINT_LIGHTS];
uniform vec3 LightColor[MAX_POINT_LIGHTS];
uniform float LightRadius[MAX_POINT_LIGHTS];
uniform int nbrPointLight;

//Dir Light
uniform int nbrDirLight;
uniform vec3 DirLightDir[10];
uniform vec3 DirLightColor[10];
uniform float DirLightBrightness[10];

uniform vec3 CamPos;

uniform bool ftr_lighting = true;
uniform bool ftr_specular = true;
uniform bool ftr_ambient = true;

float PI = 3.14159265359;

vec3 norm = normalize(texture(gNormal, UVcoords).rgb);
vec3 FragPos = texture(gPos, UVcoords).rgb;

vec3 Diffuse = texture(gAlbedo, UVcoords).rgb;
float Specular = texture(gAlbedo, UVcoords).a;
float SpecularExp = texture(gSpec, UVcoords).r;

float Rness = texture(gMetRoughAO, UVcoords).g;
float Metllc = texture(gMetRoughAO, UVcoords).r;
float AO = texture(gMetRoughAO, UVcoords).b;

vec3 viewDir = normalize(CamPos - FragPos);

vec3 F0 = mix(vec3(0.04), Diffuse, Metllc);

vec3 FresnelSchlick(float cosTheta, vec3 F0) {
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
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
	vec3 dir = normalize(vec3(-DirLightDir[index]));

	//Diffuse
	float diff = max(0.0, dot(norm, dir));
	vec3 diffuse = (diff * DirLightBrightness[index] * DirLightColor[index]);

	//Specular
	vec3 d_specular = vec3(0.0);
	if(ftr_specular) {
		vec3 reflectDir = reflect(-dir, norm);

		float spec = pow(max(0.0, dot(viewDir, reflectDir)), SpecularExp);

		d_specular = spec * DirLightColor[index] * DirLightBrightness[index] * Specular;
	}

	return diffuse + d_specular;
}

vec3 CalcPointLight(int lightIndex) {
	float dist = length(LightPos[lightIndex] - FragPos);

	if(dist < LightRadius[lightIndex]) {
		vec3 ColPow = LightColor[lightIndex] * LightBrightness[lightIndex];

		vec3 L = normalize(LightPos[lightIndex] - FragPos);
		vec3 H = normalize(viewDir + L);

		float attenuation = 1.0 / (1.0 + 0.7 * (dist * dist));
		vec3 radiance = ColPow * attenuation;

		float NDF = DistributionGGX(norm, H, Rness);
		float G = GeomSmith(norm, viewDir, L, Rness);
		vec3 F = FresnelSchlick(max(dot(H, viewDir), 0.0), F0);

		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - Metllc;

		vec3 numerator = NDF * G * F;
		float denominator = 4.0 * max(dot(norm, viewDir), 0.0) * max(dot(norm, L), 0.0);
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

	if(ftr_lighting) {
		for(int i = 0; i < nbrDirLight; i++) {
			diffSpec += CalcDirLight(i) * shadow;
		}

		for(int i = 0; i < nbrPointLight; i++) {
			diffSpec += CalcPointLight(i);
		}
	}

	vec3 ambient = (AmbientColor * AmbientStrength) * Diffuse * AO;
	vec3 result = (diffSpec + ambient);

	//Gamma
	result = result / (result + vec3(1.0));
	result = pow(result, vec3(1.0 / 2.2));

	LightPass = vec4(result, 1.0);
}