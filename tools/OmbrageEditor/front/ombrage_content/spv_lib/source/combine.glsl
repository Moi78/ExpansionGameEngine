#version 450

vec4 combine4v(float x, float y, float z, float w) {
    return vec4(x, y, z, w);
}

vec3 combine3v(float x, float y, float z) {
    return vec3(x, y, z);
}

vec2 combine2v(float x, float y) {
    return vec2(x, y);
}

void main() {
    combine2v(0, 0);
    combine3v(0, 0, 0);
    combine4v(0, 0, 0, 0);
}