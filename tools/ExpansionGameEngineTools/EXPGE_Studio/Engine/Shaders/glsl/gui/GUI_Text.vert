#version 410
layout (location = 0) in vec3 vertex;
layout (location = 2) in vec2 aUV;

out vec2 UVcoords;

uniform mat4 proj;
uniform mat4 mdl;

void main() {
	gl_Position = proj * mdl * vec4(vertex, 1.0);
	UVcoords = aUV;
}