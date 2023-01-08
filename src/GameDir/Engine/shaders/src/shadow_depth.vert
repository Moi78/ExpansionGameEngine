#version 450
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec4 aBWeights;
layout (location = 4) in vec4 aBonesID;

layout (binding = 7) uniform LIGHT_MAT {
    mat4 lmats[10];
};

layout (push_constant) uniform MODEL {
    mat4 model;
    int bone_offset;
    int idx_;
};

layout (binding = 1) uniform BONES {
    mat4 bones[1024];
};

void main() {
    vec3 fragPos;
    if(bone_offset != -1) {
        vec3 totalPos = vec3(0.0);

        for (int i = 0; i < 4; i++) {
            if(aBonesID[i] == -1) {
                continue;
            }

            int bID = int(aBonesID[i]);
            mat4 bone = bones[bone_offset + bID];

            float weight = aBWeights[i];
            totalPos += (vec4(aPos, 1.0) * bone).xyz * weight;
        }

        fragPos = (vec4(totalPos, 1.0) * model).xyz;
    } else {
        fragPos = (vec4(aPos, 1.0) * model).xyz;
    }

    gl_Position = vec4(fragPos, 1.0) * lmats[idx_];
}