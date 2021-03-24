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

mat4 Rotate(vec3 rot, mat4 src) {
    mat4 s = src;

    float sTx = sin(radians(rot.x));
    float cTx = cos(radians(rot.x));

    float sTy = sin(radians(rot.y));
    float cTy = cos(radians(rot.y));

    mat4 mx = mat4(
        1, 0, 0, 0,
        0, cTx, -sTx, 0,
        0, sTx, cTx, 0,
        0, 0, 0, 1
    );

    s = mx * src;

    mat4 my = mat4(
        cTy, 0, sTy, 0,
        0, 1, 0, 0,
        -sTy, 0, cTy, 0,
        0, 0, 0, 1
    );

    s = my * src;

    return s;
}

void main()
{
    Normal = normalize(mat3(transpose(inverse(model))) * aNormal);

    FragPos = vec3(model * transpose(pMat) * vec4(aPos, 1.0));
    UVcoord = aUV;

    gl_Position = transpose(projection) * transpose(view) * vec4(FragPos, 1.0);
}