#pragma once

#ifndef _VEC4_H__
#define _VEC4_H__

#include <valarray>

#include "vec3.h"

template<typename T>
class vec4 {
public:
	vec4(T x = 0, T y = 0, T z = 0, T w = 0) {
		m_xyzw[0] = x;
		m_xyzw[1] = y;
		m_xyzw[2] = z;
		m_xyzw[3] = w;
	}

	vec4(vec3<T> a, T w = 1) {
		m_xyzw[0] = a.getX();
		m_xyzw[1] = a.getY();
		m_xyzw[2] = a.getZ();
		m_xyzw[3] = w;
	}

	T GetX() { return m_xyzw[0]; };
	T GetY() { return m_xyzw[1]; };
	T GetZ() { return m_xyzw[2]; };
	T GetW() { return m_xyzw[3]; };

	void SetX(T nx) { m_xyzw[0] = nx; };
	void SetY(T ny) { m_xyzw[1] = ny; };
	void SetZ(T nz) { m_xyzw[2] = nz; };
	void SetW(T nw) { m_xyzw[3] = nw; };

	void SetAll(T x, T y, T z, T w) {
		m_xyzw = { x, y, z, w };
	}

	float DotProduct(vec4<T> a) {
		std::valarray<T> fa = { m_xyzw[0], m_xyzw[1], m_xyzw[2], m_xyzw[3] };
		std::valarray<T> fb = { (double)a.GetX(),(double)a.GetY(),(double)a.GetZ(),(double)a.GetW() };

		float result = (fa * fb).sum();

		return result;
	}

	void NormalizeVector() {
		float length = (float)abs(sqrt(m_xyzw[0] * m_xyzw[0]) + (m_xyzw[1] * m_xyzw[1]) + (m_xyzw[2] * m_xyzw[2] + (m_xyzw[3] * m_xyzw[3])));

		m_xyzw[0] /= length;
		m_xyzw[1] /= length;
		m_xyzw[2] /= length;
		m_xyzw[3] /= length;
	}

	bool operator==(vec4<T> const& a) {
		return m_xyzw[0] == a.m_xyzw[0] && m_xyzw[1] == a.m_xyzw[1] && m_xyzw[2] == a.m_xyzw[2] && m_xyzw[3] == a.m_xyzw[3] ? true : false;
	}

	bool operator!=(vec4<T> const& a) {
		return m_xyzw[0] == a.m_xyzw[0] && m_xyzw[1] == a.m_xyzw[1] && m_xyzw[2] == a.m_xyzw[2] && m_xyzw[3] == a.m_xyzw[3] ? false : true;
	}

	vec4<T> operator+(vec4<T> const& a) {
		return vec4<T>(m_xyzw[0] + a.m_xyzw[0], m_xyzw[1] + a.m_xyzw[1], m_xyzw[2] + a.m_xyzw[2], m_xyzw[3] + a.m_xyzw[3]);
	}

	vec4<T> operator+=(vec4<T> const& a) {
		return vec4<T>(m_xyzw[0] + a.m_xyzw[0], m_xyzw[1] + a.m_xyzw[1], m_xyzw[2] + a.m_xyzw[2], m_xyzw[3] + a.m_xyzw[3]);
	}

	vec4<T> operator-(vec4<T> const& a) {
		return vec4<T>(m_xyzw[0] - a.m_xyzw[0], m_xyzw[1] - a.m_xyzw[1], m_xyzw[2] - a.m_xyzw[2], m_xyzw[3] - a.m_xyzw[3]);
	}

	vec4<T> operator*(vec4<T> const& a) {
		return vec4<T>(m_xyzw[0] * a.m_xyzw[0], m_xyzw[1] * a.m_xyzw[1], m_xyzw[2] * a.m_xyzw[2], m_xyzw[3] * a.m_xyzw[3]);
	}

	vec4<T> operator*(float a) {
		return vec4<T>(m_xyzw[0] * a, m_xyzw[1] * a, m_xyzw[2] * a, m_xyzw[3] * a);
	}

	vec3<T> XYZ() {
		return vec3<T>(m_xyzw[0], m_xyzw[1], m_xyzw[2]);
	}

	float* GetPTR() {
		return m_xyzw;
	}

	void DBGPrint() {
		std::cout << "X : " << m_xyzw[0] << " Y : " << m_xyzw[1] << " Z : " << m_xyzw[2] << " W : " << m_xyzw[3] << std::endl;
	}
	
private:
	T m_xyzw[4];
};

typedef vec4<float> vec4f;
typedef vec4<int> vec4i;
typedef vec4<double> vec4d;

#endif //_VEC4__