#version 450 core
layout (location = 1) out vec3 ssr_color;

in vec2 UVcoords;

uniform sampler2D gPos;
uniform sampler2D gNorm;
uniform sampler2D ShadedImg;
uniform sampler2D gMetRoughAO;
uniform sampler2D Depth;

layout(std140, binding = 0) uniform CAMERA {
    mat4 projection;
    mat4 view;
};

layout(std140, binding = 5) uniform CameraData { 
    vec3 camPos;
};

void main() {
    discard;
}