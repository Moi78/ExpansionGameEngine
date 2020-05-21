#pragma once

#ifndef _ROTATOR_H__
#define _ROTATOR_H__

#ifdef _WIN32
#ifdef EXPANSIONMATH_EXPORTS
#define EXPANSIONMATH_API __declspec(dllexport)
#else
#define EXPANSIONMATH_API __declspec(dllimport)
#endif

#else

#define EXPANSIONMATH_API

#endif //_WIN32

#include "vec3.h"

class EXPANSIONMATH_API rotator
{
public:
	rotator(float angle = 0.0f, vec3f axis = vec3f(0.0f, 0.0f, 1.0f));
	~rotator();

	float GetAngle();
	vec3f GetAxis();

private:
	float m_angle;
	vec3f m_axis;
};

#endif //_ROTATOR_H__