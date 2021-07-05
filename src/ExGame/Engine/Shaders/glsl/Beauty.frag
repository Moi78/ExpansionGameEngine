#version 450 core
#extension GL_ARB_bindless_texture : enable
out vec4 FragColor;

in vec2 UVcoords;

#ifndef GL_ARB_bindless_texture
    uniform sampler2D lightpass;
    uniform sampler2D bufferA;
    uniform sampler2D bufferB;
#else
    layout(std430, binding = 12) buffer BINDLESS_FINAL_PASSES {
        sampler2D fpasses[4];
    };
#endif

layout(std430, binding = 20) buffer FINAL_PASS_SELECTOR {
    int selector;
};

void main() {
    #ifndef GL_ARB_bindless_texture
        vec4 light_bloom = texture(lightpass, UVcoords);
    #else
        vec4 light_bloom = texture(fpasses[selector], UVcoords);
    #endif
    FragColor = vec4(light_bloom.rgb, 1.0);
}