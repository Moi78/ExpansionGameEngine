#ifndef _BASIC_MATH_H__
#define _BASIC_MATH_H__

#include "pch.h"

#ifdef _WIN32
#ifdef EXPANSIONMATH_EXPORTS
#define EXPANSIONMATH_API __declspec(dllexport)
#else
#define EXPANSIONMATH_API __declspec(dllimport)
#endif

#else

#define EXPANSIONMATH_API

#endif //_WIN32

#define PI 3.14159265359
#define DEG_TO_RAD(x) (x * PI / 180)

bool EXPANSIONMATH_API isBetweenf(float inf, float toCompare, float sup);
bool EXPANSIONMATH_API isBetweeni(int inf, int toCompare, int sup);

#endif
