#version 450
layout (location = 0) out vec4 oColor;
layout (location = 1) out vec4 oNorm;
layout (location = 2) out vec4 oPos;

layout (location = 0) in vec2 UVcoords;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec3 FragPos;

void main() {
    oColor = vec4(0.0, 1.0, 0.5, 1.0);
    oNorm = vec4(norm, 1.0);
    oPos = vec4(FragPos, 1.0);
}