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

void main() {
    vec4 light_bloom = texture(fpasses[selector], UVcoords);
    FragColor = vec4(light_bloom.rgb, 1.0);
}