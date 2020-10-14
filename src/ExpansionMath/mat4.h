#pragma once

#ifndef _MAT4_H__
#define _MAT4_H__

#ifdef _WIN32
#ifdef EXPANSIONMATH_EXPORTS
#define EXPANSIONMATH_API __declspec(dllexport)
#else
#define EXPANSIONMATH_API __declspec(dllimport)
#endif

#else

#define EXPANSIONMATH_API

#endif //_WIN32

#include <iostream>
#include <algorithm>
#include <cstring>

#include "vec4.h"

class EXPANSIONMATH_API mat4
{
public:
	mat4(float init_val = 1.0f);
	~mat4();

	float GetValAt(int row, int column);

	void DBG_print_matrix();

	void operator*(float);
	void operator*(vec4f const&);

private:
	float m_mat[16];
};

#endif // _MAT4_H__
