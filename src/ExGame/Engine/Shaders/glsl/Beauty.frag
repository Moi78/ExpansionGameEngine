#version 450 core
#extension GL_ARB_bindless_texture : enable

in vec2 UVcoords;

uniform sampler2D lightpass;
uniform sampler2D GUIscreen;

uniform sampler2D bloom;
uniform sampler2D SSR;

out vec4 FragColor;

void main() {
    vec4 gui = texture(GUIscreen, UVcoords);

    //FragColor = mix(texture(lightpass, UVcoords), gui, gui.a);

    vec4 light_bloom = clamp(texture(lightpass, UVcoords), 0, 1) + (texture(bloom, UVcoords) * 2);

    FragColor = mix(light_bloom, gui, gui.a);
}