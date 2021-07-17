#version 410
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aUV;

uniform mat4 mdl;
uniform mat4 proj;

out vec2 UVcoords;

void main() {
	UVcoords = aUV;

	gl_Position = (proj * mdl) * vec4(aPos, 1.0);
}