#version 450 core
layout (location = 0) in vec3 aPos;

layout (std140, binding = 14) uniform LIGHTCAM {
	mat4 lightview;
	mat4 lightproj;
};

layout(std140, binding = 13) uniform MODEL {
	mat4 model;
};

void main() {
	gl_Position = vec4(aPos, 1.0) * model * lightview * lightproj;
}