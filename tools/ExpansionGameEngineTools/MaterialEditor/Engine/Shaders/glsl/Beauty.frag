#version 410 core

in vec2 UVcoords;

uniform sampler2D lightpass;
uniform sampler2D GUIscreen;
uniform sampler2D SSR;
uniform sampler2D gMetRoughAO;

out vec4 FragColor;

void main() {
    vec4 gui = texture(GUIscreen, UVcoords);

    vec4 lightSFX = mix(texture(lightpass, UVcoords), texture(SSR, UVcoords), texture(gMetRoughAO, UVcoords).r);

    FragColor = clamp(mix(lightSFX, gui, gui.a), 0.0, 1.0);
}