#version 450
layout (location = 0) out vec4 oColor;
layout (location = 1) out vec4 oNorm;
layout (location = 2) out vec4 oPos;
layout (location = 3) out vec4 oMetRoughAO;
layout (location = 4) out vec4 oSheen;

layout (location = 0) in vec2 UVcoords;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec3 FragPos;
layout (location = 3) in vec3 Tan;
layout (location = 4) in vec3 Bitan;

layout(binding = 2) uniform sampler2D nmap;

vec3 GetNormFromMap(vec3 NMap, vec3 n, vec3 FPos, vec2 UV) {
    vec3 tanNorm = NMap * 2.0 - 1.0;

    vec3 Q1 = dFdx(FPos);
    vec3 Q2 = dFdy(FPos);
    vec2 st1 = dFdx(UV);
    vec2 st2 = dFdy(UV);

    vec3 Nm = normalize(n);
    vec3 T = normalize(Q1 * st2.t - Q2 * st1.t);
    vec3 B = -normalize(cross(Nm, T));
    mat3 TBN = mat3(T, B, Nm);

    return normalize(TBN * tanNorm);
}

void main() {
    oColor = vec4(1, 1, 1, 1.0);
    oNorm = vec4(GetNormFromMap(texture(nmap, UVcoords).xyz, norm, FragPos, UVcoords), 0.0);
    //oNorm = vec4(bump(vec4(0.5, 0.5, 1, 0), norm, Tan, Bitan), 1);
    //oNorm = vec4(norm, 1.0);
    oPos = vec4(FragPos, 1.0);

    oMetRoughAO = vec4(0.0, 1.0, 0.3, 0.0);
    oSheen = vec4(0.0, 0.0, 0.0, 0.0);
}