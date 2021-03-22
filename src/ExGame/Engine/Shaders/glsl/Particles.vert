#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 displace;
layout (location = 4) in vec3 angles;

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

    float sTx = sin(rot.x);
    float cTx = cos(rot.x);

    float sTy = sin(rot.y);
    float cTy = cos(rot.y);

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

    vec3 p = aPos + displace;

    mat4 rot = Rotate(angles, mat4(1.0));

    FragPos = vec3(model * vec4(p, 1.0));
    UVcoord = aUV;

    gl_Position = transpose(projection) * transpose(view) * vec4(FragPos, 1.0);
}