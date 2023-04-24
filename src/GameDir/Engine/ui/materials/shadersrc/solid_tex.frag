#version 450

layout (location = 0) out vec4 oColor;

layout (location = 0) in vec2 UVcoords;
layout (location = 1) in vec2 FragPos;
layout (location = 2) in vec4 Color;

layout (binding = 3) uniform sampler2D tex;

void main() {
    oColor = texture(tex, UVcoords);
}
