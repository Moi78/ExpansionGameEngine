#version 450
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec4 aBWeights;
layout (location = 4) in vec4 aBonesID;

layout (binding = 7) uniform LIGHT_MAT {
    mat4 lmats[10];
};

layout (push_constant) uniform MODEL {
    mat4 model;
    int idx_;
};

void main() {
    gl_Position = (vec4(aPos, 1.0) * model) * lmats[idx_];
}