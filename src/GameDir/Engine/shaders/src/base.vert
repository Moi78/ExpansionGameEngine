#version 450
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec4 aBWeights;
layout (location = 4) in vec4 aBonesID;

layout (location = 0) out vec2 UVcoords;
layout (location = 1) out vec3 norm;
layout (location = 2) out vec3 FragPos;

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
    mat3 normalMatrix = transpose(inverse(mat3(model_)));

    vec3 totalPos = vec3(0.0);
    vec3 totalNorm = vec3(0.0);

    vec3 fragPos = vec3(0.0);
    if(bone_offset != -1) {
        for (int i = 0; i < 4; i++) {
            if(aBonesID[i] == -1) {
                continue;
            }

            int bID = int(aBonesID[i]);
            mat4 bone = bones[bone_offset + bID];

            float weight = aBWeights[i];
            totalPos += (vec4(aPos, 1.0) * bone).xyz * weight;
            totalNorm += aNorm * transpose(inverse(mat3(bone))) * weight;
        }

        fragPos = (vec4(totalPos, 1.0) * model_).xyz;
        norm = totalNorm;
    } else {
        fragPos = (vec4(aPos, 1.0) * model_).xyz;
        norm = normalize(aNorm * normalMatrix);
    }

    gl_Position = vec4(fragPos, 1.0) * view * proj;

    UVcoords = aUV;
    FragPos = fragPos;
}