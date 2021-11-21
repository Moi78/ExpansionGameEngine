#version 450 core
#extension GL_ARB_bindless_texture : enable
layout (location = 0) out vec3 gPos;
layout (location = 1) out vec3 gNorm;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out float gSpec;
layout (location = 4) out vec3 gMetRoughAO;
layout (location = 5) out vec3 gEmissive;

in vec3 Normal;
in vec3 Tan;
in vec3 Bitan;

in vec3 FragPos;
in vec2 UVcoord;

layout(std430) buffer TEXTURES {
  sampler2D tex[1];  
};

#define TEXTURE(id, index, UV) texture(tex[index], UV)

void main() {
    gPos = FragPos;
    gNorm = normalize(Normal);
    gAlbedo = vec4(0.0, 0.0, 0.0, 0.0);
    gSpec = 0;
    gMetRoughAO = vec3(0.0, 0.0, 0.0);
    gEmissive = vec3(texture(tex[0], UVcoord).r, texture(tex[0], UVcoord).r, texture(tex[0], UVcoord).r);
}