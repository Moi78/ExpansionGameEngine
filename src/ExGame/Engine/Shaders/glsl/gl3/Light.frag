#version 420 core
layout (location = 0) out vec4 LightPass;

in vec2 UVcoords;

//uniform sampler2D GUIscreen;

//Passes
uniform sampler2D ShadowPass;

uniform sampler2D gPos;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gSpec;

//Ambient
layout(std140, binding = 2) uniform AMBIENT{
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

layout(std140, binding = 5) uniform CameraPos {
	vec3 CamPos;
};

uniform bool ftr_lighting;
uniform bool ftr_specular;
uniform bool ftr_ambient;

vec3 norm = normalize(texture(gNormal, UVcoords).rgb);
vec3 FragPos = texture(gPos, UVcoords).rgb;

vec3 Diffuse = texture(gAlbedo, UVcoords).rgb;
float Specular = texture(gAlbedo, UVcoords).a;
float SpecularExp = texture(gSpec, UVcoords).r;

vec3 viewDir = normalize(CamPos - FragPos);

vec3 CalcDirLight(int index) {
	vec3 dir = normalize(vec3(-dlights[index].Dir));

	//Diffuse
	float diff = max(0.0, dot(norm, dir));
	vec3 diffuse = (diff * dlights[index].brightness * dlights[index].Color);

	//Specular
	vec3 d_specular = vec3(0.0);
	if(ftr_specular) {
		vec3 reflectDir = reflect(-dir, norm);

		float spec = pow(max(0.0, dot(viewDir, reflectDir)), SpecularExp);

		d_specular = spec * dlights[index].Color * dlights[index].brightness * Specular;
	}

	return diffuse + d_specular;
}

vec3 CalcPointLight(int lightIndex) {
	float dist = length(plights[lightIndex].LightPos - FragPos);
	if(dist < plights[lightIndex].LightRadius) {
		//Diffuse
		vec3 LightDir = normalize(plights[lightIndex].LightPos - FragPos);
		float attenuation = (1.0 / (1.0 + 0.7 * (dist * dist)));

		float diffuse = plights[lightIndex].LightBrightness * max(0.0, dot(norm, LightDir)) / dist;

		//Specular
		vec3 specular = vec3(0.0, 0.0, 0.0);

		if(ftr_specular) {
			vec3 reflectDir = reflect(-LightDir, norm);

			vec3 hwDir = normalize(LightDir + viewDir);
			float spec = pow(max(0.0, dot(norm, hwDir)), SpecularExp);

			specular = (spec * plights[lightIndex].LightColor * Specular);
		}

		return attenuation * (plights[lightIndex].LightColor * (diffuse + specular));
	} else {
		return vec3(0.0);
	}
}

void main() {
	if(!ftr_lighting) {
		LightPass = texture(gAlbedo, UVcoords);
		return;
	}

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

	vec4 render = clamp(pow(vec4(Diffuse, 1.0) * vec4(result, 1.0), gamma), 0.0, 1.0);
	render.a = 1.0;

	LightPass = render;
}