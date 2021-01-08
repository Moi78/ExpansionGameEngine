#ifndef _VEC3_H__
#define _VEC3_H__

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

#include <valarray>
#include <string>
#include <iostream>
#include <algorithm>

template<typename T>
class vec3 {
public:
    vec3(T x = 0, T y = 0, T z = 0) {
        m_xyz[0] = x;
        m_xyz[1] = y;
        m_xyz[2] = z;
    }

    ~vec3() {}

    T getX() { return m_xyz[0]; }
    T getY() { return m_xyz[1]; }
    T getZ() { return m_xyz[2]; }
    
    
    void setX(T x) { m_xyz[0] = x; }
    void setY(T y) { m_xyz[1] = y; }
    void setZ(T z) { m_xyz[2] = z; }

    void setAll(T x, T y, T z) {
        m_xyz[0] = x;
        m_xyz[1] = y;
        m_xyz[2] = z;
    }

    float dotProduct(vec3<T> a) {
        std::valarray<T> fa = { m_xyz[0], m_xyz[1], m_xyz[2] };
        std::valarray<T> fb = {(double) a.getX(),(double) a.getY(),(double) a.getZ() };

        const float result = (fa * fb).sum();

        return result;
    }

    void NormalizeVector() {
        float length = (float)abs(sqrt((m_xyz[0] * m_xyz[0]) + (m_xyz[1] * m_xyz[1]) + (m_xyz[2] * m_xyz[2])));

        m_xyz[0] /= length;
        m_xyz[1] /= length;
        m_xyz[2] /= length;
    }

    bool operator==(vec3<T> const& a) {
        return m_xyz[0] == a.m_xyz[0] && m_xyz[1] == a.m_xyz[1] && m_xyz[2] == a.m_xyz[2] ? true : false;
    }

    bool operator!=(vec3<T> const& a) {
        return m_xyz[0] == a.m_xyz[0] && m_xyz[1] == a.m_xyz[1] && m_xyz[2] == a.m_xyz[2] ? false : true;
    }

    vec3<T> operator+(vec3<T> const& a) {
        return vec3<T>(m_xyz[0] + a.m_xyz[0], m_xyz[1] + a.m_xyz[1], m_xyz[2] + a.m_xyz[2]);
    }

    vec3<T> operator+=(vec3<T> const& a) {
        return vec3<T>(m_xyz[0] + a.m_xyz[0], m_xyz[1] + a.m_xyz[1], m_xyz[2] + a.m_xyz[2]);
    }

    vec3<T> operator-(vec3<T> const& a) {
        return vec3<T>(m_xyz[0] - a.m_xyz[0], m_xyz[1] - a.m_xyz[1], m_xyz[2] - a.m_xyz[2]);
    }

    vec3<T> operator*(vec3<T> const& a) {
        return vec3<T>(m_xyz[0] * a.m_xyz[0], m_xyz[1] * a.m_xyz[1], m_xyz[2] * a.m_xyz[2]);
    }

    vec3<T> operator*(float const& a) {
        return vec3<T>(m_xyz[0] * a, m_xyz[1] * a, m_xyz[2] * a);
    }

    void DBGPrint() {
        std::cout << "X : " << m_xyz[0] << " Y : " << m_xyz[1] << " Z : " << m_xyz[2] << std::endl;
    }

    T* GetPTR() {
        return m_xyz;
    }

private:
    T m_xyz[3];
};

template<class T>
vec3<T> Cross(vec3<T> a, vec3<T> b) {
    return vec3<T>(
        a.getY() * b.getZ() - a.getZ() * b.getY(),
        a.getZ() * b.getX() - a.getX() * b.getZ(),
        a.getX() * b.getY() - a.getY() * b.getX()
    );
}

typedef vec3<float> vec3f;
typedef vec3<int> vec3i;
typedef vec3<double> vec3d;

#endif //_VEC3_H__
