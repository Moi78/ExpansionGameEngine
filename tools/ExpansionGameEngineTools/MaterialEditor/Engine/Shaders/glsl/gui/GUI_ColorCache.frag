#version 410
layout (location = 0) out vec4 screen;

uniform vec3 color;
uniform float alpha;

void main() {
	screen = vec4(color, alpha);
}