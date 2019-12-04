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
uniform vec3 LightPos;
uniform vec3 LightColor = vec3(1.0, 1.0, 1.0);
uniform float LightBrightness;

//Camera
uniform vec3 CamPos;

//Specular
uniform float specularStrength = 256;
uniform vec3 specularColor = vec3(1.0, 1.0, 1.0);

//Features define
uniform bool ftr_lighting = false;
uniform bool ftr_specular = false;
uniform bool ftr_texture = false;
uniform bool ftr_ambient = true;

void main()
{
	//Ambient color
	vec3 ambient = vec3(0.0, 0.0, 0.0);

	if(ftr_ambient) {
		ambient = AmbientStrength * AmbientColor;
	}

	//Light Vars Default
	vec3 LightDir;
	vec3 norm;
	vec3 diffuse;

	//Specular
	vec3 specular;

	float attenuation = 1.0;

	if(ftr_lighting == true) {
		norm = normalize(Normal);
		LightDir = normalize(LightPos - FragPos);
		float diff = max(0.0, dot(norm, LightDir));
		
		float dist = length(LightPos - FragPos);
		attenuation = (1.0 / (1.0 + 0.7 * dist + 1.8 * (dist * dist)));

		diffuse = (diff * vec3(1.0, 1.0, 1.0) * LightBrightness * LightColor);
	} else {
		diffuse = vec3(1.0, 1.0, 1.0);
	}

	if(ftr_specular == true) {
		vec3 viewDir = normalize(CamPos - FragPos);

		vec3 reflectDir = reflect(-LightDir, norm);

		float spec = pow(max(0.0, dot(viewDir, reflectDir)), distance(FragPos, LightPos));

		specular = (spec * LightColor * specularColor) / distance(FragPos, LightPos);
	} else {
		specular = vec3(0.0, 0.0, 0.0);
	}

	vec3 result = BaseColor * ((attenuation*(diffuse)) + specular + ambient);

    FragColor = vec4(result, 1.0);
}