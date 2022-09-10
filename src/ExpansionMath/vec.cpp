#include "vec.h"

vec3 Cross(vec3 a, vec3 b) {
    vec3 res = vec3(0.0, 0.0, 0.0);

    res.SetX(a.GetY() * b.GetZ() - a.GetZ() * b.GetY());
    res.SetY(a.GetZ() * b.GetX() - a.GetX() * b.GetZ());
    res.SetZ(a.GetX() * b.GetY() - a.GetY() * b.GetX());

    return res;
}