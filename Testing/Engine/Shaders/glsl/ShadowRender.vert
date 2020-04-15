#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUVcoords;

out vec2 UVcoords;
out vec3 Normal;

out vec4 FragPosLightSpace;
out vec4 FragPos;

uniform mat4 lspaceMat;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//Useless but avoid the internal error
uniform vec3 CamPos;

void main() {
	UVcoords = aUVcoords;
	FragPos = model * vec4(aPos, 1.0);
	Normal = transpose(inverse(mat3(model))) * aNormal;

	FragPosLightSpace = vec4(aPos, 1.0) * lspaceMat;

	gl_Position = FragPos * projection * view;
}