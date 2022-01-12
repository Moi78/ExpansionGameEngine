#version 450 core
#extension GL_ARB_bindless_texture : enable
layout (location = 0) out vec3 ssr_color;

in vec2 UVcoords;

layout(std140, binding = 0) uniform CAMERA {
    mat4 projection;
    mat4 view;
};

layout(std140, binding = 5) uniform CameraData { 
    vec3 camPos;
};

layout(std430, binding = 8) buffer BINDLESS_PASSES {
    sampler2D passes[6];
};

//layout(std430, binding = 19) buffer BINDLESS_ENV_CUBEMAP {
//    samplerCube env_cubemap;
//};

uniform samplerCube env_Cubemap;

void main() {
    vec3 norm = normalize(texture(passes[1], UVcoords).rgb);
    vec3 FragPos = texture(passes[0], UVcoords).rgb;
    vec3 I = normalize(camPos - FragPos);

    vec3 norm_r = reflect(I, norm);
    vec3 norm_r_sw = vec3(norm.x, norm.z, norm.y);

    float Spec = texture(passes[3], UVcoords).r;

    ssr_color = texture(env_Cubemap, norm_r_sw).rgb * Spec;
}