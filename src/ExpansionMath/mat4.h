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

#define MAKE_MM128_SCALAR(x) {a, a, a, a}

#include <iostream>
#include <algorithm>
#include <cstring>

#include "vec4.h"

#include <immintrin.h>
#include <cstring>

template<class T>
class mat4
{
public:
	mat4(T init_val) {
		memset(m_mat, 0, sizeof(m_mat));

		if (sizeof(T) == sizeof(float)) {
			_simd_enabled = true;
		}
		else {
			_simd_enabled = false;
		}

		/*
		* Making identity matrix

		ival,	0.0f,	0.0f,	0.0f,
		0.0f,	ival,	0.0f,	0.0f,
		0.0f,	0.0f,	ival,	0.0f,
		0.0f,	0.0f,	0.0f,	ival,

		*/

		for (int i = 0; i < 16; i += 5) {
			m_mat[i] = init_val;
		}
	}

	~mat4() {

	}

	float GetValAt(int row, int column) {
		return m_mat[4 * row + column];
	}

	void DBG_print_matrix() {
		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				std::cout << m_mat[4 * x + y] << ", ";
			}

			std::cout << std::endl;
		}
	}

	vec4<T> operator*(vec4<T> a) {
		if (_simd_enabled) {
			__m128 vec = { a.GetX(), a.GetY(), a.GetZ(), a.GetW() };

			T XYZW[4];
			for (int i = 0; i < 16; i += 4) {
				__m128 mat_row = { m_mat[i] , m_mat[i + 1], m_mat[i + 2], m_mat[i + 3] };
				__m128 vec_new = _mm_mul_ps(mat_row, vec);

				XYZW[i / 4] = vec_new.m128_f32[0] + vec_new.m128_f32[1] + vec_new.m128_f32[2] + vec_new.m128_f32[3];
			}

			return vec4<T>(XYZW[0], XYZW[1], XYZW[2], XYZW[3]);
		}
		else {
			T XYZW[4];
			for (int i = 0; i < 16; i += 4) {
				XYZW[i / 4] = a.GetX() * m_mat[i] + a.GetY() * m_mat[i + 1] + a.GetZ() * m_mat[i + 2] + a.GetW() * m_mat[i + 3];
			}

			return vec4<T>(XYZW[0], XYZW[1], XYZW[2], XYZW[3]);
		}
	}

	mat4<T> operator*(float a) {
		if (_simd_enabled) {
			__m128 scalar = MAKE_MM128_SCALAR(a);
			for (int i = 0; i < 16; i += 4) {
				__m128 m = { m_mat[i], m_mat[i + 1], m_mat[i + 2], m_mat[i + 3] };
				__m128 mmultiplied = _mm_mul_ps(m, scalar);

				memcpy(&m_mat[i], mmultiplied.m128_f32, 4 * sizeof(float));
			}
		}
		else {
			for (int i = 0; i < 16; i++) {
				m_mat[i] *= a;
			}
		}

		return *this;
	}

	mat4<T> operator*(mat4<T> const& a) {

	}

private:
	T m_mat[16];

	bool _simd_enabled;
};

typedef mat4<float> mat4f;

#endif // _MAT4_H__
