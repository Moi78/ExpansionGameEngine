#version 450
layout (location = 0) out vec4 oColor;

layout (location = 0) in vec2 UVcoords;
layout (location = 1) in vec2 FragPos;
layout (location = 2) in vec4 Color;
layout (location = 3) flat in int texID;

layout (binding = 3) uniform sampler2D texs[3];

void main() {
    oColor = texture(texs[texID], UVcoords);
}