#version 450 core
#extension GL_ARB_bindless_texture : enable
layout (location = 1) out float ssao_blur;

in vec2 UVcoords;

layout(std430, binding = 9) buffer BINDLESS_SFX {
    sampler2D passes_sfx[5];
};

void main() {
    vec2 texelSize = 1.0 / vec2(textureSize(passes_sfx[3], 0));

    float result = 0.0;
    for(int x = -2; x < 2; x++) {
        for(int y = -2; y < 2; y++) {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(passes_sfx[3], UVcoords + offset).r;
        }
    }

    ssao_blur = clamp(result / (4.0 * 4.0), 0.0, 1.0);
}