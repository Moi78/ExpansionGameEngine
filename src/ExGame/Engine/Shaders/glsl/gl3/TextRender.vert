#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

out vec3 Normal;
out vec3 FragPos;
out vec2 UVcoords;

layout (std140, binding = 0) uniform CAMERA {
	mat4 proj;
	mat4 view;
};

layout (std140, binding = 13) uniform MODEL {
	mat4 model;
};

void main() {
	Normal = normalize(mat3(transpose(inverse(model))) * aNormal);

	FragPos = vec3(vec4(aPos, 1.0) * model);
	UVcoords = aUV;

	gl_Position = vec4(FragPos, 1.0) * view * proj;
}