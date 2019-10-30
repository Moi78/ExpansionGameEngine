#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 BaseColor;

uniform float AmbientStrength;
uniform vec3 AmbientColor;

uniform vec3 LightPos;
uniform float LightBrightness;

uniform bool ftr_lighting = false;
uniform bool ftr_specular = false;
uniform bool ftr_texture = false;
uniform bool ftr_ambient = false;

void main()
{
	vec3 diffuse;

	//Ambient color
	vec3 ambient = AmbientStrength * AmbientColor;

	if(ftr_lighting == true) {
		vec3 norm = normalize(Normal);
		vec3 LightDir = normalize(LightPos - FragPos);
		float diff = clamp(dot(norm, LightDir), 0.0, 1.0);
		
		diffuse = (diff * vec3(1.0, 1.0, 1.0) * LightBrightness) / distance(FragPos, LightPos);
	} else {
		diffuse = vec3(1.0, 1.0, 1.0);
	}

	if(ftr_specular == true) {
		
	}

	vec3 result = BaseColor * (ambient + diffuse);

    FragColor = vec4(result, 1.0);
	//FragColor = vec4(normalize(Normal), 1.0);
}