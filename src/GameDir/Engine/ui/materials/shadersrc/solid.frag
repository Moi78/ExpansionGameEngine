#version 450

layout (location = 0) out vec4 oColor;

layout (location = 0) in vec2 UVcoords;
layout (location = 1) in vec2 FragPos;

void main() {
    oColor = vec4(UVcoords, 0.0, cos(distance(FragPos, vec2(0, 0)) * 3));
}
