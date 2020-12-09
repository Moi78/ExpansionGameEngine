#version 410 core

layout (location = 0) in vec3 aPos;

uniform mat4 lightproj;
uniform mat4 lightview;
uniform mat4 model;

void main() {
	gl_Position = lightproj * lightview * model * vec4(aPos, 1.0);
}