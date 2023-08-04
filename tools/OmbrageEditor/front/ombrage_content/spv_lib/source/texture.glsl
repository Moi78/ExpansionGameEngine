#version 450

layout(binding = 0) uniform sampler2D t;

vec4 sampleTex(vec2 uv, sampler2D tex) {
    return texture(tex, uv);
}

void main() {
    sampleTex(vec2(0), t);
}
