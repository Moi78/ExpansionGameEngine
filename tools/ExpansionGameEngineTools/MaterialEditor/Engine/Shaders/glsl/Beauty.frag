#version 450 core
#extension GL_ARB_bindless_texture : enable

in vec2 UVcoords;

#ifndef GL_ARB_bindless_texture
    uniform sampler2D lightpass;
    uniform sampler2D GUIscreen;
    uniform sampler2D bloom;
#else
    layout(std430, binding = 12) buffer BINDLESS_FINAL_PASSES {
        sampler2D fpasses[4];
    };
#endif

//uniform sampler2D SSR;

out vec4 FragColor;

void main() {
    #ifndef GL_ARB_bindless_texture
        vec4 gui = texture(GUIscreen, UVcoords);
        vec4 light_bloom = clamp(clamp(texture(lightpass, UVcoords), 0, 1) + (texture(bloom, UVcoords) * 2), 0, 1);
    #else
        vec4 gui = texture(fpasses[2], UVcoords);
        vec4 light_bloom = clamp(clamp(texture(fpasses[0], UVcoords), 0, 1) + (texture(fpasses[2], UVcoords) * 2), 0, 1);
    #endif

    //FragColor = mix(light_bloom, gui, gui.a);
    FragColor = (light_bloom);
}