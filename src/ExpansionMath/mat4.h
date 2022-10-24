#pragma once

#ifndef _MAT4_H__
#define _MAT4_H__

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <math.h>
#include <immintrin.h>

#include "vec.h"
#include "SIMD_Utils.h"

#define PI 3.14159265359
#define DEG_TO_RAD(x) ((x) * PI / 180)

template<class T>
class mat4
{
public:
	mat4(T init_val = 1) {
		memset(m_mat, 0, 16 * sizeof(T));

		/*
		    Making identity matrix

		ival,	0.0f,	0.0f,	0.0f,
		0.0f,	ival,	0.0f,	0.0f,
		0.0f,	0.0f,	ival,	0.0f,
		0.0f,	0.0f,	0.0f,	ival,

		*/

		for (int i = 0; i < 16; i += 5) {
			m_mat[i] = init_val;
		}
	}

	mat4(T mat[16]) {
		memset(m_mat, 0, sizeof(m_mat));
		memcpy(m_mat, mat, 16 * sizeof(T));
	}

	~mat4() {

	}

	float GetValAt(int row, int column) {
		return m_mat[4 * row + column];
	}

    vec3 GetTranslation() {
        return vec3(m_mat[3], m_mat[7], m_mat[11]);
    }

	void DBG_print_matrix() {
		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				std::cout << m_mat[4 * x + y] << ", ";
			}

			std::cout << std::endl;
		}
	}

	vec4 operator*(vec4 a) {
        Float4 vec(a.GetX(), a.GetY(), a.GetZ(), a.GetW());

        T XYZW[4];
        for (int i = 0; i < 16; i += 4) {
            Float4 mat_row(m_mat[i] , m_mat[i + 1], m_mat[i + 2], m_mat[i + 3]);
			Float4 vec_new = mat_row * vec;

			XYZW[i / 4] = vec_new.Get(0) + vec_new.Get(1) + vec_new.Get(2) + vec_new.Get(3);
        }

        return vec4(XYZW[0], XYZW[1], XYZW[2], XYZW[3]);
	}

	mat4<T> operator*(float a) {
		T nMat[16];
		memset(nMat, 0, 16 * sizeof(T));

        Float4 scalar;
        scalar.LoadConstant(a);
        for (int i = 0; i < 16; i += 4) {
            Float4 m(m_mat[i], m_mat[i + 1], m_mat[i + 2], m_mat[i + 3]);
            Float4 mmultiplied = m * scalar;

            memcpy(&nMat[i], mmultiplied.GetPointer(), 4 * sizeof(float));
        }

		return mat4<T>(nMat);
	}

	mat4<T> operator*(mat4<T> const& a) {
        //SIMD Impl.

        //Allocating matrix in XMM registers
        std::vector<Float4> mat_1_rows;
		for (int i = 0; i < 16; i += 4) {
			mat_1_rows.push_back(Float4(m_mat[i], m_mat[i + 1], m_mat[i + 2], m_mat[i + 3]));
		}

		//Declaring result mat
		T nMat[16];

		for (int i = 0; i < 4; i++) {
			for (int c = 0; c < 4; c++) {
				//Getting column of the second matrix
				Float4 col(a.m_mat[c], a.m_mat[c + 4], a.m_mat[c + 8], a.m_mat[c + 12]);

				//Multiplying & storing results
				Float4 result = mat_1_rows[i] * col;

				//Sum up everything
				T comp = 0;
				for (int s = 0; s < 4; s++) {
					comp += result.Get(s);
				}

				nMat[4 * i + c] = comp;
			}
		}

		return mat4<T>(nMat);
	}

	T* GetPTR() {
		return &m_mat[0];
	}

private:
	T m_mat[16];
};

template<class T>
mat4<T> TranslateMatrix(mat4<T> srcMat, vec3 trans) {
	T transMat[16] = {
		1, 0, 0, trans.GetX(),
		0, 1, 0, trans.GetY(),
		0, 0, 1, trans.GetZ(),
		0, 0, 0, 1
	};
	mat4<T> tmat(transMat);

	return srcMat * tmat;
}

template<class T>
mat4<T> ScaleMatrix(mat4<T> srcMat, vec3 scale) {
	T scaleMat[16] = {
		scale.GetX(), 0			  , 0			, 0			   ,
		0			, scale.GetY(), 0			, 0			   ,
		0			, 0			  , scale.GetZ(), 0			   ,
		0			, 0			  ,	0			, 1
	};
	mat4<T> smat(scaleMat);

	return srcMat * scaleMat;
}

template<class T>
mat4<T> RotateMatrix(mat4<T> srcMat, vec3 rot) {
	mat4<T> srcCopy = srcMat;

	float X = DEG_TO_RAD(rot.GetX());
	float Y = DEG_TO_RAD(rot.GetY());
	float Z = DEG_TO_RAD(rot.GetZ());

	//X
	float Stheta = sin(X);
	float Ctheta = cos(X);

	T rxMat[16] = {
	1,		0,		 0, 0,
	0, Ctheta, -Stheta, 0,
	0, Stheta,  Ctheta, 0,
	0,		0,		 0, 1
	};

	srcCopy = srcCopy * mat4<T>(rxMat);

	//Y
	Stheta = sin(Y);
	Ctheta = cos(Y);

	T ryMat[16] = {
		 Ctheta,	 0, Stheta,		0,
			  0,	 1,		 0,		0,
		-Stheta,	 0, Ctheta,		0,
			  0,	 0,		 0,		1
	};

	srcCopy = srcCopy * mat4<T>(ryMat);

	//Z
	Stheta = sin(Z);
	Ctheta = cos(Z);

	T rzMat[16] = {
		Ctheta, -Stheta, 0, 0,
		Stheta,  Ctheta, 0, 0,
			 0,		  0, 1, 0,
			 0,		  0, 0, 1
	};

	srcCopy = srcCopy * mat4<T>(rzMat);

	return srcCopy;
}

mat4<float> LookAt(vec3 pos, vec3 target, vec3 up);
mat4<float> ProjPersp(float FOV, float ImageRatio, float nearv = 0.1f, float farv = 1000.0f);
mat4<float> ProjOrtho(float right, float left, float top, float bottom, float nearv, float farv);

typedef mat4<float> mat4f;

#endif // _MAT4_H__
