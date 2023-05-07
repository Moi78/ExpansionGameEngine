#version 450
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec4 aBWeights;
layout (location = 4) in vec4 aBonesID;

layout (location = 0) out vec2 UVcoords;
layout (location = 1) out vec2 FragPos;
layout (location = 2) out vec4 Color;
layout (location = 3) out int texID;

struct Rect {
    float x;
    float y;
    float w;
    float h;
};

layout (binding = 81) uniform SCREEN_MAT {
    Rect absScreen;
};

layout(push_constant) uniform RECT_COLOR_ID {
    Rect r;
    vec4 color;
    int id;
};

void main() {
    UVcoords = aUV;

    vec2 pos = aPos.xy * vec2(r.w / absScreen.w, r.h / absScreen.h);
    pos += vec2(r.w / (absScreen.w), r.h / (absScreen.h));
    pos += 2 * vec2(r.x / absScreen.w, r.y / absScreen.h);
    pos -= vec2(1.0, 1.0);

    gl_Position = vec4(pos, 0.0, 1.0);
    FragPos = pos;
    Color = color;

    texID = id;
}