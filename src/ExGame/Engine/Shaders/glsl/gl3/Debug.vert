#version 450 core
layout (location = 0) in vec3 aPos;

layout(std140, binding = 0) uniform CAMERA {
    mat4 projection;
    mat4 view;
};

layout(std140, binding = 13) uniform MODEL {
    mat4 model;  
};

void main()
{
    vec3 FragPos = vec3(vec4(aPos, 1.0) * model);

    gl_Position = vec4(FragPos, 1.0) * view * projection;
}