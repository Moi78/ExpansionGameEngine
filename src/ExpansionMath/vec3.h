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

template<typename T>
class vec3 {
public:
    vec3(T x = 0, T y = 0, T z = 0) {
        m_x = x;
        m_y = y;
        m_z = z;
    }

    ~vec3() {
    
    }

    T getX() {
        return m_x;
    }

    T getY() {
        return m_y;
    }
    
    T getZ() {
        return m_z;
    }

    void setX(T x) {
        m_x = x;
    }

    void setY(T y) {
        m_y = y;
    }

    void setZ(T z) {
        m_z = z;
    }

    void setAll(T x, T y, T z) {
        m_x = x;
        m_y = y;
        m_z = z;
    }

    float dotProduct(vec3<T> a) {
        std::valarray<T> fa = { m_x, m_y, m_z };
        std::valarray<T> fb = {(double) a.getX(),(double) a.getY(),(double) a.getZ() };

        float result = (fa * fb).sum();

        return result;
    }

    void NormalizeVector() {
        float length = (float)abs(sqrt((m_x * m_x) + (m_y * m_y) + (m_z * m_z)));

        m_x /= length;
        m_y /= length;
        m_z /= length;
    }

    bool operator==(vec3<T> const& a) {
        return m_x == a.m_x && m_y == a.m_y && m_z == a.m_z ? true : false;
    }

    bool operator!=(vec3<T> const& a) {
        return m_x == a.m_x && m_y == a.m_y && m_z == a.m_z ? false : true;
    }

    vec3<T> operator+(vec3<T> const& a) {
        return vec3<T>(m_x + a.m_x, m_y + a.m_y, m_z + a.m_z);
    }

    vec3<T> operator+=(vec3<T> const& a) {
        return vec3<T>(m_x + a.m_x, m_y + a.m_y, m_z + a.m_z);
    }

    vec3<T> operator-(vec3<T> const& a) {
        return vec3<T>(m_x - a.m_x, m_y - a.m_y, m_z - a.m_z);
    }

    vec3<T> operator*(vec3<T> const& a) {
        return vec3<T>(m_x * a.m_x, m_y * a.m_y, m_z * a.m_z);
    }

    vec3<T> operator*(float const& a) {
        return vec3<T>(m_x * a, m_y * a, m_z * a);
    }

    void DBGPrint() {
        std::cout << "X : " << m_x << " Y : " << m_y << " Z : " << m_z << std::endl;
    }

private:
    T m_x;
    T m_y;
    T m_z;
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
