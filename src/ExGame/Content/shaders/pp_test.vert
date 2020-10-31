#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

out vec2 UVcoords;

void main() {
    gl_Position = vec4(aPos, 0.0);

    UVcoords = aUV;
}