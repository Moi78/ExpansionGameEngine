#version 450
layout (location = 0) out vec4 oColor;

layout (location = 0) in vec2 UVcoords;

layout (binding = 1) uniform sampler2D tex;
layout (binding = 2) uniform sampler2D gradient;

void main() {
    vec3 nColor = (texture(tex, UVcoords).rgb) * (texture(gradient, UVcoords).rgb);

    oColor = vec4(nColor, 1.0);
}
