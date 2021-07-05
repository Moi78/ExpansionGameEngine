#version 450 core
#extension GL_ARB_bindless_texture : enable
out vec4 FragColor;

in vec2 UVcoords;

#ifndef GL_ARB_bindless_texture
    uniform sampler2D lightpass;
#else
    layout(std430, binding = 12) buffer BINDLESS_FINAL_PASSES {
        sampler2D fpasses[4];
    };
#endif

void main() {
    #ifndef GL_ARB_bindless_texture
        vec4 light_bloom = texture(lightpass, UVcoords);
    #else
        vec4 light_bloom = texture(fpasses[0], UVcoords);
    #endif
    FragColor = vec4(light_bloom.rgb, 1.0);
}