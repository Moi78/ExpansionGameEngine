#version 410 core
layout (location = 1) out vec3 ssr_color;

in vec2 UVcoords;

uniform sampler2D gPos;
uniform sampler2D gNorm;
uniform sampler2D ShadedImg;
uniform sampler2D gMetRoughAO;
uniform sampler2D Depth;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 camPos;

void main() {
    discard;
}