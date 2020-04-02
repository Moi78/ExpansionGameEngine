#version 410 core

out vec4 FragColor;

//Input vertex data
in vec3 Normal;
in vec3 FragPos;
in vec2 UVcoord;
in vec4 FragPosLightSpace;

uniform sampler2D ShadowMap;
uniform sampler2D BaseColor;

//Ambient Lighting
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

float processShadow() {
	vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;

	if(projCoords.z > 1.0) {
		return 0.0;
	}

	float closestDepth = texture(ShadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;

	float bias = max(0.05 * (1.0 - dot(norm, DirLightDir[0])), 0.005);

	float shadow = 0.0;

	vec2 texelSize = 1.0 / textureSize(ShadowMap, 0);
	for(int x = -1; x <= 1; x++) {
		for(int y = -1; y <= 1; y++) {
			float pcfDepth = texture(ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}

	return shadow / 9;
}

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

		vec3 hwDir = normalize(LightDir + viewDir);
		float spec = pow(max(0.0, dot(norm, hwDir)), specularExp);

		specular = (spec * LightColor[lightIndex] * LightBrightness[lightIndex] * specularColor * specularStrength);
	} else {
		specular = vec3(0.0, 0.0, 0.0);
	}

	return attenuation * (diffuse + specular);
}

vec3 CalcDirLight(int index) {
	vec3 dir = normalize(vec3(DirLightDir[index].xy, -DirLightDir[index].z));

	//Diffuse
	float diff = max(0.0, dot(dir, norm));
	vec3 diffuse = (diff * DirLightBrightness[index] * DirLightColor[index]);

	//Specular
	vec3 d_specular = vec3(0.0);

	if(ftr_specular) {
		vec3 reflectDir = reflect(vec3(dir.xy, -dir.z), norm);

		float spec = pow(max(0.0, dot(viewDir, reflectDir)), specularExp);

		d_specular = spec * DirLightColor[index] * specularColor * DirLightBrightness[index] * specularStrength;
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

	vec3 result = (diffSpec * (1.0 - processShadow()) + ambient);

	vec4 gamma = vec4(1.0 / 2.2);
    FragColor = pow(texture(BaseColor, UVcoord)  * vec4(result, 1.0), gamma);
}