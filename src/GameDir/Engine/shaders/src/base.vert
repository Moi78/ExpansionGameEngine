#version 450
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aUV;

layout (location = 0) out vec2 UVcoords;
layout (location = 1) out vec3 norm;
layout (location = 2) out vec3 FragPos;

layout (binding = 0) uniform TRANFORMS {
    mat4 proj;
    mat4 view;
    mat4 model;
};

void main() {
    vec3 fragPos = (vec4(aPos, 1.0) * model).xyz;
    gl_Position = vec4(fragPos, 1.0) * view * proj;

    UVcoords = aUV;

    mat3 normalMatrix = transpose(inverse(mat3(model)));
    norm = aNorm * normalMatrix;

    FragPos = fragPos;
}