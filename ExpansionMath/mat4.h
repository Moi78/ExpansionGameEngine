#pragma once

#ifndef _MAT4_H__
#define _MAT4_H__

#ifdef EXPANSIONMATH_EXPORTS
#define EXPANSIONMATH_API __declspec(dllexport)
#else
#define EXPANSIONMATH_API __declspec(dllimport)
#endif

#include <iostream>
#include <algorithm>

class EXPANSIONMATH_API mat4
{
public:
	mat4(double filler);
	mat4(double mat[4][4]);
	~mat4();

	double GetValAt(int row, int column);

	void DBG_print_matrix();

	mat4 operator*(mat4 const& a);

private:
	double m_mat[4][4];
};

#endif // _MAT4_H__