#version 410
layout (location = 0) in vec4 vertex;

out vec2 UVcoords;

uniform mat4 proj;

void main() {
	gl_Position = proj * vec4(vertex.xy, 1.0, 1.0);
	UVcoords = vec2(vertex.zw);
}