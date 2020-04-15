#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

out vec3 Normal;
out vec3 FragPos;
out vec2 UVcoord;
out vec4 FragPosLightSpace;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform mat4 lspaceMat;

void main()
{
    Normal = normalize(mat3(transpose(inverse(model))) * aNormal);
    FragPos = vec3(model * vec4(aPos, 1.0));
    UVcoord = aUV;
    FragPosLightSpace = lspaceMat * vec4(FragPos, 1.0);

    gl_Position = projection * view * vec4(FragPos, 1.0);
}