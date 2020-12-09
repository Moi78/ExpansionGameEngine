#version 410 core

in vec2 UVcoords;

uniform sampler2D lightpass;
uniform sampler2D GUIscreen;

out vec4 FragColor;

void main() {
    vec4 gui = texture(GUIscreen, UVcoords);

    FragColor = clamp(mix(texture(lightpass, UVcoords), gui, gui.a), 0.0, 1.0);
}