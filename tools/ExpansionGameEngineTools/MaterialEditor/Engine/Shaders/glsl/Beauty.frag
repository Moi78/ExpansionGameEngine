#version 410 core

in vec2 UVcoords;

uniform sampler2D lightpass;
uniform sampler2D GUIscreen;

uniform sampler2D gMetRoughAO;
uniform sampler2D SSR;

out vec4 FragColor;

void main() {
    vec4 gui = texture(GUIscreen, UVcoords);

    //FragColor = mix(texture(lightpass, UVcoords), gui, gui.a);

    vec4 lightSSR = texture(SSR, UVcoords);

    FragColor = mix(lightSSR, gui, gui.a);
}