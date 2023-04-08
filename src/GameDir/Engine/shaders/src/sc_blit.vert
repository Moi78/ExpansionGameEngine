#version 450
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec4 aBWeights;
layout (location = 4) in vec4 aBonesID;

layout (location = 0) out vec2 UVcoords;

layout(binding = 80) uniform VIEWPORT {
    float x;
    float y;
    float w;
    float h;
};

layout(push_constant) uniform FULLSCREEN_OVR {
    bool fullscreenOverride;
};

void main() {
    if(!fullscreenOverride) {
        vec2 pos = aPos.xy * vec2(w, h);
        pos += vec2(x, y);

        gl_Position = vec4(pos, 0.0, 1.0);
    } else {
        gl_Position = vec4(aPos.xy, 0.0, 1.0);
    }

    UVcoords = aUV;
}
