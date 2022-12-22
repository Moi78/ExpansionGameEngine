#version 450
layout (location = 0) out float exp_cz;

void main() {
    float depth = gl_FragCoord.z / gl_FragCoord.w;
    depth = depth * 0.5 + 0.5;

    const float c = 80;
    exp_cz = exp(-c * depth);
}