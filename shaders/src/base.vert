#version 450
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aUV;

layout (location = 0) out vec2 UVcoords;

layout (binding = 0) uniform CAMERA {
    mat4 proj;
    mat4 view;
};

void main() {
    gl_Position = vec4(aPos, 1.0) * view * proj;

    UVcoords = aUV;
}