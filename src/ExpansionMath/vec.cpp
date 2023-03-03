#include "vec.h"

vec3 Cross(vec3 a, vec3 b) {
    vec3 res = vec3(0.0, 0.0, 0.0);

    res.SetX(a.GetY() * b.GetZ() - a.GetZ() * b.GetY());
    res.SetY(a.GetZ() * b.GetX() - a.GetX() * b.GetZ());
    res.SetZ(a.GetX() * b.GetY() - a.GetY() * b.GetX());

    return res;
}

vec3 Lerp(vec3& a, vec3& b, float t) {
    float x = a.GetX() * (1 - t) + b.GetX() * t;
    float y = a.GetY() * (1 - t) + b.GetY() * t;
    float z = a.GetZ() * (1 - t) + b.GetZ() * t;

    return vec3(x, y, z);
}