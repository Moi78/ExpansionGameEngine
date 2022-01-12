#version 450 core
out vec4 FragColor;

in vec2 UVcoords;

uniform sampler2D lightpass;

layout(std430, binding = 15) buffer FINAL_PASS_SELECTOR {
    int selector;
};

void main() {
    vec4 light_bloom = texture(lightpass, UVcoords);
    FragColor = vec4(light_bloom.rgb, 1.0);
}