#version 450
layout (location = 0) out float shadow;

layout (binding = 0) uniform LIGHT_MAT {
    mat4 lmats[10];
};

layout (binding = 5) uniform CASTER_COUNT {
    int nDLights;
    int nPLights;
};

layout (binding = 1) uniform sampler2D textures[10];

void main() {

}