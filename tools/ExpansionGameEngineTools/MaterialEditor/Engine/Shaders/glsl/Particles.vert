#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in mat4 pMat;

out vec3 Normal;

out vec3 FragPos;
out vec2 UVcoord;

layout(std140, binding = 0) uniform CAMERA {
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main()
{
    Normal = normalize(mat3(transpose(inverse(model))) * aNormal);

    FragPos = vec3(model * transpose(pMat) * vec4(aPos, 1.0));
    UVcoord = aUV;

    gl_Position = vec4(FragPos, 1.0) * view * projection;
}