#version 450 core
#extension GL_ARB_bindless_texture : enable
layout (location = 0) out float ssao;

in vec2 UVcoords;

#ifndef GL_ARB_bindless_texture
    uniform sampler2D gPos;
    uniform sampler2D gNorm;
#else
    layout(std430, binding = 8) buffer BINDLESS_PASSES {
        sampler2D passes[6];
    };
#endif //GL_ARB_bindless_texture

uniform sampler2D noise;

layout(std140, binding = 0) uniform CAMERA {
    mat4 projection;
    mat4 view;
};

layout(std140, binding = 6) uniform SSAO_Data {
    int scr_w;
    int scr_h;

    vec3 samples[64];
};

float radius = 0.5;
float bias = 0.05;

vec2 noiseScale = vec2(scr_w / 4.0, scr_h / 4.0);

void main() {
    #ifdef GL_ARB_bindless_texture
        vec3 FragPos = vec4(texture(passes[0], UVcoords) * view).xyz;
        vec3 norm = texture(passes[1], UVcoords).xyz;
    #else
        vec3 FragPos = vec4(texture(gPos, UVcoords) * view).xyz;
        vec3 norm = texture(gNorm, UVcoords).xyz;
    #endif //GL_ARB_bindless_texture

    vec3 randomVec = normalize(texture(noise, UVcoords * noiseScale).xyz);

    vec3 tangent = normalize(randomVec - norm * dot(randomVec, norm));
    vec3 bitangent = cross(norm, tangent);
    mat3 TBN = mat3(tangent, bitangent, norm);

    float occlusion = 0.0;
    for(int i = 0; i < 64; i++) {
        vec3 samplePos = TBN * samples[i];
        samplePos = samplePos * radius + FragPos;

        vec4 offset = vec4(samplePos, 1.0);
        offset = offset * projection;
        offset.xy /= offset.w;
        offset.xy = offset.xy * 0.5 + 0.5;

        float sampleDepth = vec4(texture(passes[0], offset.xy) * view).z;

        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(FragPos.z - sampleDepth));
        occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;
    }

    occlusion = 1.0 - (occlusion / 64.0);
    ssao = occlusion;
}