#version 450
layout (location = 0) out vec4 oColor;

layout (location = 0) in vec2 UVcoords;

layout (binding = 0) uniform vec3 color;

void main() {
    oColor = vec4(color, 1.0);
}
