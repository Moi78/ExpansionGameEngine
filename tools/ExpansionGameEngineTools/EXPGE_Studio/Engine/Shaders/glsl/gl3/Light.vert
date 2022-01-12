#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aUV;

out vec2 UVcoords;

void main() {
	UVcoords = aUV;
	gl_Position = vec4(aPos, 1.0);
}