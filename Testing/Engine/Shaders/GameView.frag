#version 410 core

out vec4 FragColor;

//Input vertex data
in vec3 Normal;
in vec3 FragPos;
in vec2 UVcoord;

uniform sampler2D BaseColor;

//Amnient Lighting
uniform float AmbientStrength;
uniform vec3 AmbientColor;

//Lighting
uniform int nbrPointLight = 0;
uniform int nbrDirLight = 0;

//Directionnal Lighting
uniform vec3 DirLightDir[10];
uniform vec3 DirLightColor[10];
uniform float DirLightBrightness[10];

//Point Lighting
uniform vec3 LightPos[327];
uniform vec3 LightColor[327];
uniform float LightBrightness[327];

//Camera
uniform vec3 CamPos;

//Specular
uniform float specularExp = 256.0;
uniform vec3 specularColor = vec3(1.0, 1.0, 1.0);
uniform float specularStrength = 1.0f;

//Features define
uniform bool ftr_lighting = true;
uniform bool ftr_specular = true;
uniform bool ftr_ambient = true;

vec3 viewDir = normalize(CamPos - FragPos);
vec3 norm = normalize(Normal);

vec3 CalcPointLighting(int lightIndex) {
	//Diffuse
	vec3 LightDir = normalize(LightPos[lightIndex] - FragPos);
	float diff = max(dot(norm, LightDir), 0.0);
		
	float dist = length(LightPos[lightIndex] - FragPos[lightIndex]);
	float attenuation = (1.0 / (1.0 + 0.7 * (dist * dist)));

	vec3 diffuse = (diff * LightBrightness[lightIndex] * LightColor[lightIndex]) / dist;

	//Specular
	vec3 specular = vec3(1.0, 1.0, 1.0);

	if(ftr_specular) {
		vec3 reflectDir = reflect(-LightDir, norm);

		float spec = pow(max(0.0, dot(viewDir, reflectDir)), specularExp);

		specular = (spec * LightColor[lightIndex] * LightBrightness[lightIndex] * specularColor * specularStrength);
	} else {
		specular = vec3(0.0, 0.0, 0.0);
	}

	return attenuation * (diffuse + specular);
}

vec3 CalcDirLight(int index) {
	vec3 dir = normalize(-DirLightDir[index]);

	//Diffuse
	float diff = max(0.0, dot(norm, dir));
	vec3 diffuse = (diff * DirLightBrightness[index] * DirLightColor[index]);

	//Specular
	vec3 d_specular = vec3(0.0);

	if(ftr_specular) {
		vec3 reflectDir = reflect(-dir, normalize(Normal));
		float spec = pow(max(0.0, dot(viewDir, reflectDir)), specularStrength);
		d_specular = spec * DirLightColor[index] * specularColor;
	}

	return (diffuse + d_specular);
}

void main()
{
	//Ambient color
	vec3 ambient = vec3(0.0, 0.0, 0.0);

	if(ftr_ambient) {
		ambient = AmbientStrength * AmbientColor;
	}

	//Lighting
	vec3 diffSpec;

	if(ftr_lighting) {
		for(int i = 0; i < nbrPointLight; i++) {
			diffSpec += CalcPointLighting(i);
		}
		for(int i = 0; i < nbrDirLight; i++) {
			diffSpec += CalcDirLight(i);
		}
	} else {
		diffSpec = vec3(1.0, 1.0, 1.0);
	}

	vec3 result = (diffSpec + ambient);

	vec4 gamma = vec4(1.0 / 2.2);
    FragColor = pow(texture(BaseColor, UVcoord)  * vec4(result, 1.0), gamma);
}