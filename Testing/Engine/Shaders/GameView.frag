#version 330 core

out vec4 FragColor;

//Input vertex data
in vec3 Normal;
in vec3 FragPos;

uniform vec3 BaseColor;

//Amnient Lighting
uniform float AmbientStrength;
uniform vec3 AmbientColor;

//Lighting
uniform int nbrPointLight = 0;

uniform bool dirLightPresent = false;

uniform vec3 LightPos[500];
uniform vec3 LightColor[500];
uniform float LightBrightness[500];

//Camera
uniform vec3 CamPos;

//Specular
uniform float specularStrength = 256.0;
uniform vec3 specularColor = vec3(1.0, 1.0, 1.0);

//Features define
uniform bool ftr_lighting = false;
uniform bool ftr_specular = false;
uniform bool ftr_texture = false;
uniform bool ftr_ambient = true;

vec3 CalcPointLighting(int lightIndex) {
	//Diffuse
	vec3 norm = normalize(Normal);
	vec3 LightDir = normalize(LightPos[lightIndex] - FragPos);
	float diff = max(0.0, dot(norm, LightDir));
		
	float dist = length(LightPos[lightIndex] - FragPos[lightIndex]);
	float attenuation = (1.0 / (1.0 + 0.7 * dist + 1.8 * (dist * dist)));

	vec3 diffuse = (diff * vec3(1.0, 1.0, 1.0) * LightBrightness[lightIndex] * LightColor[lightIndex]);

	//Specular
	vec3 specular = vec3(1.0, 1.0, 1.0);

	if(ftr_specular) {
		vec3 viewDir = normalize(CamPos - FragPos);

		vec3 reflectDir = reflect(-LightDir, norm);

		//distance(FragPos, LightPos[lightIndex])

		float spec = pow(max(0.0, dot(viewDir, reflectDir)), specularStrength);

		specular = (spec * LightColor[lightIndex] * specularColor) / distance(FragPos, LightPos[lightIndex]);
	} else {
		specular = vec3(0.0, 0.0, 0.0);
	}

	return (attenuation * diffuse) + specular;
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

	if(ftr_lighting == true) {
		for(int i = 0; i < nbrPointLight; i++) {
			diffSpec += CalcPointLighting(i);
		}
	} else {
		diffSpec = vec3(1.0, 1.0, 1.0);
	}

	vec3 result = BaseColor * (diffSpec + ambient);

    FragColor = vec4(result, 1.0);
}