#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

out vec3 Normal;

out vec3 FragPos;
out vec2 UVcoord;

layout(std140, binding = 0) uniform CAMERA {
    mat4 projection;
    mat4 view;
};

layout(std140, binding = 13) uniform MODEL {
    mat4 model;
};

void main()
{
    Normal = normalize(aNormal * mat3(transpose(inverse(model))));

    FragPos = vec3(vec4(aPos, 1.0) * model);
    UVcoord = aUV;

    gl_Position = vec4(FragPos, 1.0) * view * projection;
}