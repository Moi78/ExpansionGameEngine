#ifndef _VEC_2_H__
#define _VEC_2_H__

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

template<class T>
class vec2 {
public:
    vec2(T x = 0, T y = 0) {
        m_x = x;
        m_y = y;
    }

    ~vec2() {

    }

    T getX() const {
        return m_x;
    }

    void setX(T x) {
        m_x = x;
    }

    T getY() const {
        return m_y;
    }

    void setY(T y) {
        m_y = y;
    }

    void setAll(T x, T y) {
        m_x = x;
        m_y = y;
    }

    float dotProduct(vec2<T> const& a) {
        const std::valarray<double> fa = { m_x, m_y };
        const std::valarray<double> fb = { (double)a.getX(),(double)a.getY() };

        const float result = (fa * fb).sum();

        return result;
    }

    bool operator==(vec2<T> const& a) {
        return m_x == a.m_x && m_y == a.m_y ? true : false;
    }

    bool operator!=(vec2<T> const& a) {
        return m_x == a.m_x && m_y == a.m_y ? false : true;
    }

    vec2<T> operator+(vec2<T> const& a) {
        return vec2<T>(m_x + a.m_x, m_y + a.m_y);
    }

    vec2<T> operator-(vec2<T> const& a) {
        return vec2<T>(m_x - a.m_x, m_y - a.m_y);
    }
    
private:
    T m_x;
    T m_y;
};

typedef vec2<float> vec2f;
typedef vec2<double> vec2d;
typedef vec2<int> vec2i;

#endif
