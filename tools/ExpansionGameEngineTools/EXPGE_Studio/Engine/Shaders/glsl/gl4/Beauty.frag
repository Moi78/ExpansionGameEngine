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
    vec4 light_bloom = texture(fpasses[selector], UVcoords);

    vec2 metrough = texture(passes[4], UVcoords).rg;

    vec4 reflections = light_bloom + (light_bloom * texture(sfx_passes[5], UVcoords) * metrough.r);
    reflections = reflections + (texture(sfx_passes[5], UVcoords) * (1 - metrough.g));

    FragColor = vec4(reflections.rgb, 1.0);
}