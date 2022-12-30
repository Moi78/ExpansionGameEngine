#version 450
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec4 aBWeights;
layout (location = 4) in vec4 aBonesID;

layout (location = 0) out vec2 UVcoords;

void main() {
    gl_Position = vec4(aPos, 1.0);

    UVcoords = aUV;
}
