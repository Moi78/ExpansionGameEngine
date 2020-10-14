#pragma once

#ifndef _VEC4_H__
#define _VEC4_H__

#include <valarray>

template<typename T>
class vec4 {
public:
	vec4(T x = 0, T y = 0, T z = 0, T w = 0) {
		m_x = x;
		m_y = y;
		m_z = z;
		m_w = w;
	}

	/*vec4(vec3<T> const& a, T w = 0) {
		m_x = a.m_x;
		m_y = a.m_y;
		m_z = a.m_z;
		m_w = w;
	}*/

	T GetX() { return m_x; };
	T GetY() { return m_y; };
	T GetZ() { return m_z; };
	T GetW() { return m_z; };

	void SetX(T nx) { m_x = nx; };
	void SetY(T ny) { m_y = ny; };
	void SetZ(T nz) { m_z = nz; };
	void SetW(T nw) { m_w = nw; };

	void SetAll(T x, T y, T z, T w) {
		m_x = x;
		m_y = y;
		m_z = z;
		m_w = w;
	}

	float DotProduct(vec4<T> a) {
		std::valarray<T> fa = { m_x, m_y, m_z, m_w };
		std::valarray<T> fb = { (double)a.GetX(),(double)a.GetY(),(double)a.GetZ(),(double)a.GetW() };

		float result = (fa * fb).sum();

		return result;
	}

	void NormalizeVector() {
		float length = (float)abs(sqrt(m_x * m_x) + (m_y * m_y) + (m_z * m_z));
		
		m_x /= length;
		m_y /= length;
		m_z /= length;
		m_w /= length;
	}

	bool operator==(vec4<T> const& a) {
		return m_x == a.m_x && m_y == a.m_y && m_z == a.m_z && m_w == a.m_w ? true : false;
	}

	bool operator!=(vec4<T> const& a) {
		return m_x == a.m_x && m_y == a.m_y && m_z == a.m_z && m_w == a.m_w ? false : true;
	}

	vec4<T> operator+(vec4<T> const& a) {
		return vec4<T>(m_x + a.m_x, m_y + a.m_y, m_z + a.m_z, m_w + a.m_w);
	}

	vec4<T> operator+=(vec4<T> const& a) {
		return vec4<T>(m_x + a.m_x, m_y + a.m_y, m_z + a.m_z, m_w + a.m_w);
	}

	vec4<T> operator-(vec4<T> const& a) {
		return vec4<T>(m_x - a.m_x, m_y - a.m_y, m_z - a.m_z, m_w - a.m_w);
	}

	vec4<T> operator*(vec4<T> const& a) {
		return vec4<T>(m_x * a.m_x, m_y * a.m_y, m_z * a.m_z, m_w * a.m_w);
	}

	vec4<T> operator*(float a) {
		return vec4<T>(m_x * a, m_y * a, m_z * a, m_w * a);
	}

	void DBGPrint() {
		std::cout << "X : " << m_x << " Y : " << m_y << " Z : " << m_z << std::endl;
	}
	
private:
	T m_x;
	T m_y;
	T m_z;
	T m_w;
};

typedef vec4<float> vec4f;
typedef vec4<int> vec4i;
typedef vec4<double> vec4d;

#endif //_VEC4__