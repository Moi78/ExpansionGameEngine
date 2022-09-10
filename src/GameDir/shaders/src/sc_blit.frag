#version 450
layout (location = 0) out vec4 oColor;

layout (location = 0) in vec2 UVcoords;

layout (binding = 0) uniform sampler2D tex;

void main() {
    oColor = texture(tex, UVcoords);
}
