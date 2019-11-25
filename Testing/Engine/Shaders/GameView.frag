#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 BaseColor;

uniform float AmbientStrength;
uniform vec3 AmbientColor;

uniform vec3 LightPos;
uniform vec3 LightColor = vec3(1.0, 1.0, 1.0);
uniform float LightBrightness;

uniform vec3 CamPos;

uniform float specularStrength = 256;

uniform bool ftr_lighting = false;
uniform bool ftr_specular = false;
uniform bool ftr_texture = false;
uniform bool ftr_ambient = false;

void main()
{
	//Ambient color
	vec3 ambient = AmbientStrength * AmbientColor;

	//Light Vars Default
	vec3 LightDir;
	vec3 norm;
	vec3 diffuse;

	//Specular
	vec3 specular;

	if(ftr_lighting == true) {
		norm = normalize(Normal);
		LightDir = normalize(LightPos - FragPos);
		float diff = max(0.0, dot(norm, LightDir));
		
		diffuse = (diff * vec3(1.0, 1.0, 1.0) * LightBrightness * LightColor) / distance(FragPos, LightPos);
	} else {
		diffuse = vec3(1.0, 1.0, 1.0);
	}

	if(ftr_specular == true) {
		vec3 viewDir = normalize(CamPos - FragPos);

		vec3 reflectDir = reflect(norm, -LightDir);

		float spec = pow(max(0.0, dot(viewDir, reflectDir)), distance(FragPos, LightPos));

		specular = (spec * LightColor) / distance(FragPos, LightPos);
	} else {
		specular = vec3(0.0, 0.0, 0.0);
	}

	vec3 result = BaseColor * (ambient + diffuse + specular);

    FragColor = vec4(result, 1.0);
}