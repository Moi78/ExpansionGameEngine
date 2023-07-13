#version 450

// Add

float addf(float a, float b) {
    return a + b;
}

vec2 add2v(vec2 a, vec2 b) {
    return a + b;
}

vec3 add3v(vec3 a, vec3 b) {
    return a + b;
}

vec4 add4v(vec4 a, vec4 b) {
    return a + b;
}

vec2 addf2v(float a, vec2 b) {
    return a + b;
}

vec3 addf3v(float a, vec3 b) {
    return a + b;
}

vec4 addf4v(float a, vec4 b) {
    return a + b;
}

// Multiply

float mulf(float a, float b) {
    return a * b;
}

vec2 mul2v(vec2 a, vec2 b) {
    return a * b;
}

vec3 mul3v(vec3 a, vec3 b) {
    return a * b;
}

vec4 mul4v(vec4 a, vec4 b) {
    return a * b;
}

vec2 mulf2v(float a, vec2 b) {
    return a * b;
}

vec3 mulf3v(float a, vec3 b) {
    return a * b;
}

vec4 mulf4v(float a, vec4 b) {
    return a * b;
}

void main() {
    // ADD
    addf(0, 0);
    add2v(vec2(0), vec2(0));
    add3v(vec3(0), vec3(0));
    add4v(vec4(0), vec4(0));
    addf2v(0, vec2(0));
    addf3v(0, vec3(0));
    addf4v(0, vec4(0));

    // MUL
    mulf(0, 0);
    mul2v(vec2(0), vec2(0));
    mul3v(vec3(0), vec3(0));
    mul4v(vec4(0), vec4(0));
    mulf2v(0, vec2(0));
    mulf3v(0, vec3(0));
    mulf4v(0, vec4(0));
}