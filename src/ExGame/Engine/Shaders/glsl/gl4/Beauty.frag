#version 450 core
#extension GL_ARB_bindless_texture : enable
out vec4 FragColor;

in vec2 UVcoords;

layout(std430, binding = 12) buffer BINDLESS_FINAL_PASSES {
    sampler2D fpasses[4];
};

layout(std430, binding = 20) buffer FINAL_PASS_SELECTOR {
    int selector;
};

layout(std430, binding = 9) buffer BINDLESS_SFX_PASSES {
    sampler2D sfx_passes[6];
};

layout(std430, binding = 8) buffer BINDLESS_PASSES {
    sampler2D passes[6];
};

void main() {
    FragColor = vec4(texture(sfx_passes[0], UVcoords).rgb, 1.0);
}