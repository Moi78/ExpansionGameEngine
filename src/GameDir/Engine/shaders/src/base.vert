#version 450
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec4 aBWeights;
layout (location = 4) in vec4 aBonesID;

layout (location = 0) out vec2 UVcoords;
layout (location = 1) out vec3 norm;
layout (location = 2) out vec3 FragPos;
layout (location = 3) out vec4 weight;
layout (location = 4) out vec4 boneID;

layout (binding = 0) uniform TRANFORMS {
    mat4 proj;
    mat4 view;
    mat4 model;
};

layout (binding = 1) uniform BONES {
    mat4 bones[1024];
};

layout (push_constant) uniform MODELS {
    mat4 model_;
    int bone_offset;
};

void main() {
    vec4 totalPos;
    vec3 fragPos;
    if(bone_offset != -1) {
        for (int i = 0; i < 4; i++) {
            if(aBonesID[i] == -1) {
                continue;
            }

            int bID = int(aBonesID[i]);
            mat4 bone = bones[bone_offset + bID];

            float weight = aBWeights[i];

            vec4 localPos = vec4(aPos, 1.0) * bone;
            totalPos += localPos * weight;
        }

        fragPos = (vec4(totalPos.xyz, 1.0) * model_).xyz;
    } else {
        fragPos = (vec4(aPos, 1.0) * model_).xyz;
    }

    gl_Position = vec4(fragPos, 1.0) * view * proj;

    UVcoords = aUV;

    mat3 normalMatrix = transpose(inverse(mat3(model_)));
    norm = aNorm * normalMatrix;

    FragPos = fragPos;

    weight = aBWeights;
    boneID = aBonesID;
}