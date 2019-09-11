#ifndef _TRANSFORM_H__
#define _TRANSFORM_H__

#include "pch.h"

#ifdef EXPANSIONMATH_EXPORTS
#define EXPANSIONMATH_API __declspec(dllexport)
#else
#define EXPANSIONMATH_API __declspec(dllimport)
#endif

#include "vec2.h"
#include "vec3.h"

#include <math.h>
#include <iostream>

#define PI 3.141592653589793

namespace Transform {
    vec3f RotatePointZ(vec3f point, vec3f pointOrigin, float angle);
    vec3f RotatePointY(vec3f point, vec3f pointOrigin, float angle);
    vec3f RotatePointX(vec3f point, vec3f pointOrigin, float angle);
}
#endif
