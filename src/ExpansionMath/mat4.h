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

class EXPANSIONMATH_API mat4
{
public:
	mat4();
	mat4(float mat[4][4]);
	~mat4();

	float GetValAt(int row, int column);

	void DBG_print_matrix();

	mat4 operator*(mat4 const& a);

private:
	float m_mat[4][4];
};

#endif // _MAT4_H__
