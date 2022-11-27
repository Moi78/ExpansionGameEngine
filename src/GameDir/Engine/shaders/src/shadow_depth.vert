#version 450
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aUV;

layout (binding = 0) uniform LIGHT_MAT {
    mat4 lmat[10];
};

layout (binding = 2) uniform INDEX {
    int idx;
};

layout (push_constant) uniform MODEL {
    mat4 model;
};

void main() {
    gl_Position = (vec4(aPos, 1.0) * model) * lmat[idx];
}