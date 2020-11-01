#version 410 core
layout (location = 5) out vec3 FragColor;

in vec2 UVcoords;

uniform sampler2D screen;

void main() {
    FragColor = texture(screen, UVcoords).rgb + vec3(1.0, 0.0, 1.0);
}