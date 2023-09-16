#version 450

vec3 bump(vec4 sampledNormal, vec3 normalVector, vec3 tanVec, vec3 bitanVec) {
    vec3 nMap = normalize(sampledNormal * 2.0 - 1.0).xyz;
    mat3 tanToWorld = mat3(
        tanVec.x, bitanVec.x, normalVector.x,
        tanVec.y, bitanVec.y, normalVector.y,
        tanVec.z, bitanVec.z, normalVector.z
    );

    return nMap * tanToWorld;
}

void main() {
    bump(vec4(0), vec3(0), vec3(0), vec3(0));
}