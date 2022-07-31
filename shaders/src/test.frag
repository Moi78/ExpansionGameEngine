#version 450
layout (location = 0) out vec4 oColor;
layout (location = 0) in vec3 vColor;

void main() {
    oColor = vec4(vColor, 1.0);
}
