#version 410
layout (location = 0) in vec3 aPos;

uniform mat4 mdl;
uniform mat4 proj;

void main() {
	gl_Position = proj * mdl * vec4(aPos, 1);
}