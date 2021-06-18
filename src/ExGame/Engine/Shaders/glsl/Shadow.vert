#version 450 core

layout (location = 0) in vec3 aPos;

uniform mat4 lightproj;
uniform mat4 lightview;

layout(std140, binding = 13) uniform MODEL {
	mat4 model;
};

void main() {
	gl_Position = lightproj * lightview * transpose(model) * vec4(aPos, 1.0);
}