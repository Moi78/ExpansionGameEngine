#version 450
layout (location = 0) out vec4 oColor;
layout (location = 1) out vec4 oColor2;
layout (location = 0) in vec2 UVcoords;

void main() {
    oColor = vec4(UVcoords, 0.0, 1.0);
    oColor2 = vec4(1.0, 0.0, 0.0, 1.0);
}