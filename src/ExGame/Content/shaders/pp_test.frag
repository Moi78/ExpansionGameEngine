#version 410 core
layout (location = 5) out vec4 FragColor;

in vec2 UVcoords;

uniform sampler2D screen;

void main() {
    FragColor = texture(screen, UVcoords) + vec4(0.2, -0.1, 0.0, 0.0);
}