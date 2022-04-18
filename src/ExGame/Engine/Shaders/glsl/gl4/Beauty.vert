#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aUV;

out vec2 UVcoords;

layout(std140, binding = 20) uniform QUAD_ZONE {
	vec2 pos;
	vec2 scale;
};

void main() {
	UVcoords = aUV;
	gl_Position = vec4((aPos * vec3(scale, 0)) + vec3(pos, 0), 1.0);
}