#version 410 core
out vec4 FragColor;

in vec2 UVcoords;

uniform sampler2D gPos;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gSpec;

//Ambient
uniform float AmbientStrength;
uniform vec3 AmbientColor;

//Point Light
uniform int nbrPointLight;
uniform vec3 LightPos[327];
uniform float LightBrightness[327];
uniform vec3 LightColor[327];

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
	vec3 dir = normalize(vec3(DirLightDir[index].xy, -DirLightDir[index].z));

	//Diffuse
	float diff = max(0.0, dot(dir, norm));
	vec3 diffuse = (diff * DirLightBrightness[index] * DirLightColor[index]);

	//Specular
	vec3 d_specular = vec3(0.0);

	vec3 reflectDir = reflect(vec3(dir.xy, -dir.z), norm);

		float spec = pow(max(0.0, dot(viewDir, reflectDir)), SpecularExp);

		d_specular = spec * DirLightColor[index] * DirLightBrightness[index] * Specular;

	return diffuse + d_specular;
}

void main() {
	vec3 diffSpec;

	for(int i = 0; i < nbrDirLight; i++) {
		diffSpec += CalcDirLight(i);
	}

	vec3 ambient = AmbientColor * AmbientStrength;
	vec3 result = (diffSpec + ambient);

	vec4 gamma = vec4(1.0 / 2.2);
	FragColor = pow(vec4(Diffuse, 1.0) * vec4(result, 1.0), gamma);
}