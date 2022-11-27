#version 450
layout (location = 0) out float shadow;

layout (location = 0) in vec2 UVcoords;

layout (binding = 0) uniform LIGHT_MAT {
    mat4 lmats[10];
};

layout (binding = 5) uniform CASTER_COUNT {
    int nDLights;
    int nPLights;
};

layout(binding = 2) uniform IDX {
    int idx;
};

layout (binding = 1) uniform sampler2D dDepths[10];
layout (binding = 3) uniform sampler2D fragPos;

void main() {
    float s = 0.0;
    for(int i = 0; i < idx + 1; i++) {
        vec3 fPos = texture(fragPos, UVcoords).rgb;
        vec4 fPos_lightSpace = vec4(fPos, 1.0) * lmats[i];
        vec3 projCoord = fPos_lightSpace.xyz / fPos_lightSpace.w;

        projCoord = projCoord * 0.5 + 0.5;

        float closestDepth = texture(dDepths[i], projCoord.xy).r;
        closestDepth = closestDepth * 0.5 + 0.5;

        float currentDepth = projCoord.z;

        float bias = 0.0025;
        s += closestDepth < currentDepth - bias ? 0.5 : 0.0;
    }

    shadow = s;
}