#version 450

vec3 bump(vec4 NMap, vec3 n, vec3 FPos, vec2 UV) {
    vec3 tanNorm = NMap.xyz * 2.0 - 1.0;

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
    bump(vec4(0), vec3(0), vec3(0), vec2(0));
}