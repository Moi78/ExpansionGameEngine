#version 450

float split4v(vec4 src, int idx) {
    return src[idx];
}

float split3v(vec3 src, int idx) {
    return src[idx];
}

float split2v(vec2 src, int idx) {
    return src[idx];
}

vec3 xyz(vec4 src) {
    return src.xyz;
}

void main() {
    split4v(vec4(0), 0);
    split3v(vec3(0), 0);
    split2v(vec2(0), 0);

    xyz(vec4(0));
}