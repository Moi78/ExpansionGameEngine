#version 450

vec3 bump(vec4 sampledNormal, vec3 normalVector, vec3 fragpos, vec2 UVcoords) {
    vec3 tanNorm = sampledNormal.xyz * 2.0 - 1.0;

    vec3 Q1 = dFdx(fragpos);
    vec3 Q2 = dFdy(fragpos);
    vec2 st1 = dFdx(UVcoords);
    vec2 st2 = dFdy(UVcoords);

    vec3 N = normalize(normalVector);
    vec3 T = normalize(Q1 * st2.t - Q2 * st1.t);
    vec3 B = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tanNorm);
}

void main() {
    bump(vec4(0), vec3(0), vec3(0), vec2(0));
}