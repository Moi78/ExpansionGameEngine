#version 450
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aTan;
layout (location = 4) in vec3 aBitan;
layout (location = 5) in vec4 aBWeights;
layout (location = 6) in vec4 aBonesID;

layout (location = 0) out vec2 UVcoords;
layout (location = 1) out vec3 norm;
layout (location = 2) out vec3 FragPos;
layout (location = 3) out vec3 tan;
layout (location = 4) out vec3 bitan;

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
    vec3 totalTan = vec3(0.0);
    vec3 totalBitan = vec3(0.0);

    vec3 fragPos = vec3(0.0);
    if(bone_offset != -1) {
        for (int i = 0; i < 4; i++) {
            if(aBonesID[i] == -1) {
                continue;
            }

            int bID = int(aBonesID[i]);
            mat4 bone = bones[bone_offset + bID];
            mat3 boneNorm = transpose(inverse(mat3(bone)));

            float weight = aBWeights[i];
            totalPos += (vec4(aPos, 1.0) * bone).xyz * weight;

            totalNorm += aNorm * boneNorm * weight;
            totalTan += aTan * boneNorm * weight;
            totalBitan += aBitan * boneNorm * weight;
        }

        fragPos = (vec4(totalPos, 1.0) * model_).xyz;
        norm = totalNorm;
        tan = totalTan;
        bitan = totalBitan;
    } else {
        fragPos = (vec4(aPos, 1.0) * model_).xyz;
        vec3 N = normalize(aNorm * normalMatrix);
        norm = N;

        tan = normalize(aTan * normalMatrix);
        tan = normalize(tan - dot(tan, N) * N);

        bitan = normalize(aBitan * normalMatrix);
        //bitan = cross(norm, tan);
    }

    gl_Position = vec4(fragPos, 1.0) * view * proj;

    UVcoords = aUV;
    FragPos = fragPos;
}