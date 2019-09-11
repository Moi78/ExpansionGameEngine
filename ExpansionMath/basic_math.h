#ifndef _BASIC_MATH_H__
#define _BASIC_MATH_H__

#include "pch.h"

#ifdef EXPANSIONMATH_EXPORTS
#define EXPANSIONMATH_API __declspec(dllexport)
#else
#define EXPANSIONMATH_API __declspec(dllimport)
#endif

bool EXPANSIONMATH_API isBetweenf(float inf, float toCompare, float sup);
bool EXPANSIONMATH_API isBetweeni(int inf, int toCompare, int sup);

#endif
