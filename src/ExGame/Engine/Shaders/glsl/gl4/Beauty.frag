#version 450 core
#extension GL_ARB_bindless_texture : enable
out vec4 FragColor;

in vec2 UVcoords;

layout(std430, binding = 12) buffer BINDLESS_FINAL_PASSES {
    sampler2D fpasses[2];
};

void main() {
    FragColor = vec4(texture(fpasses[1], UVcoords).rgb, 1.0);
}