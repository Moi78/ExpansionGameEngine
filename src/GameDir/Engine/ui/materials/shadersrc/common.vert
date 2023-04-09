#version 450
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec4 aBWeights;
layout (location = 4) in vec4 aBonesID;

layout (location = 0) out vec2 UVcoords;
layout (location = 1) out vec2 FragPos;

struct Rect {
    float x;
    float y;
    float w;
    float h;
};

layout(push_constant) uniform RECT {
    Rect r;
};

void main() {
    UVcoords = aUV;

    vec2 pos = aPos.xy * vec2(r.w, r.h);
    pos += vec2(r.x, r.y);

    gl_Position = vec4(pos, 0.0, 1.0);
    FragPos = pos;
}