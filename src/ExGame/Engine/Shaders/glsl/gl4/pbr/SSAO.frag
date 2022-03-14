#version 450 core
#extension GL_ARB_bindless_texture : enable
layout (location = 0) out vec3 ssao;

in vec2 UVcoords;

layout(std430, binding = 8) buffer BINDLESS_PASSES {
    sampler2D passes[7];
};

layout(std430, binding = 11) buffer BINDLESS_SSAO {
    sampler2D noise;
};

layout(std140, binding = 0) uniform CAMERA {
    mat4 projection;
    mat4 view;
};

layout(std140, binding = 6) uniform SSAO_Data {
    int scr_w;
    int scr_h;

    vec3 samples[64];
};

layout(std140, binding = 5) uniform CamData {
	vec3 CamPos;
};

float radius = 1.0;
float bias = 0.025;

vec2 noiseScale = vec2(scr_w / 4.0, scr_h / 4.0);

void main() {
    vec4 FragPos = texture(passes[0], UVcoords);
    vec3 norm = normalize(texture(passes[1], UVcoords).xyz);

    vec3 randomVec = texture(noise, UVcoords * noiseScale).xyz * 2.0 - 1.0;

    vec3 tangent = normalize(randomVec - norm * dot(randomVec, norm));
    vec3 bitangent = cross(norm, tangent);
    mat3 TBN = mat3(tangent, bitangent, norm);

    float occlusion = 0.0;
    for(int i = 0; i < 64; i++) {
        vec3 samplePos = TBN * samples[i];
        samplePos = FragPos.xyz + samplePos * radius;
        float samplePosDepth = length(CamPos - samplePos) / 1000;

        vec4 offset = vec4(samplePos, 1.0);
        offset = projection * offset;
        offset.xyz /= offset.w;
        offset.xy = offset.xy * 0.5 + 0.5;

        float sampleDepth = length(CamPos - texture(passes[0], offset.xy).xyz) / 1000;

        occlusion += (samplePosDepth + bias <= sampleDepth) ? 1.0 : 0.0;

        //float rangeCheck = smoothstep(0.0, 1.0, radius / abs(FragPos.z - sampleDepth));
        //occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;
    }

    occlusion = 1.0 - (occlusion / 64.0);
    ssao = vec3(occlusion, 0.0, 0.0);

    //ssao = vec3(length(CamPos - texture(passes[0], UVcoords).xyz) / 1000, 0.0, 0.0);
}