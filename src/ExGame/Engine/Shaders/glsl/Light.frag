#version 410 core
//To don't mess up with max register limit
#pragma glsl

out vec4 FragColor;

in vec2 UVcoords;

uniform sampler2D gPos;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gSpec;

//Passes
uniform sampler2D ShadowPass;

//Ambient
uniform float AmbientStrength;
uniform vec3 AmbientColor;

//Point Light
const int MAX_POINT_LIGHTS = 246;
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

vec3 norm = normalize(texture(gNormal, UVcoords).rgb);
vec3 FragPos = texture(gPos, UVcoords).rgb;

vec3 Diffuse = texture(gAlbedo, UVcoords).rgb;
float Specular = texture(gAlbedo, UVcoords).a;
float SpecularExp = texture(gSpec, UVcoords).r;

vec3 viewDir = normalize(CamPos - FragPos);

vec3 CalcDirLight(int index) {
	vec3 dir = normalize(vec3(-DirLightDir[index]));

	//Diffuse
	float diff = max(0.0, dot(norm, dir));
	vec3 diffuse = (diff * DirLightBrightness[index] * DirLightColor[index]);

	//Specular
	vec3 d_specular = vec3(0.0);

	vec3 reflectDir = reflect(-dir, norm);

		float spec = pow(max(0.0, dot(viewDir, reflectDir)), SpecularExp);

		d_specular = spec * DirLightColor[index] * DirLightBrightness[index] * Specular;

	return diffuse + d_specular;
}

vec3 CalcPointLight(int lightIndex) {
	float dist = length(LightPos[lightIndex] - FragPos);
	if(dist < LightRadius[lightIndex]) {
		//Diffuse
		vec3 LightDir = normalize(LightPos[lightIndex] - FragPos);
		float attenuation = (1.0 / (1.0 + 0.7 * (dist * dist)));

		float diffuse = max(0.0, dot(norm, LightDir)) / dist;

		//Specular
		vec3 specular = vec3(1.0, 1.0, 1.0);

		vec3 reflectDir = reflect(-LightDir, norm);

		vec3 hwDir = normalize(LightDir + viewDir);
		float spec = pow(max(0.0, dot(norm, hwDir)), SpecularExp);

		specular = (spec * LightColor[lightIndex] * LightBrightness[lightIndex] * Specular);

		return attenuation * (diffuse + specular);
	} else {
		return vec3(0.0);
	}
}

void main() {
	vec3 diffSpec = vec3(0.0);

	float shadow = texture(ShadowPass, UVcoords).r;

	for(int i = 0; i < nbrDirLight; i++) {
		diffSpec += CalcDirLight(i) * shadow;
	}

	for(int i = 0; i < nbrPointLight; i++) {
		diffSpec += CalcPointLight(i);
	}

	vec3 ambient = AmbientColor * AmbientStrength;
	vec3 result = (diffSpec + ambient);

	vec4 gamma = vec4(1.0 / 2.2);
	FragColor = pow(vec4(Diffuse, 1.0) * vec4(result, 1.0), gamma);
}