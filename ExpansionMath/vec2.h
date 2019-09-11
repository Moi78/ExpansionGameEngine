#ifndef _VEC_2_H__
#define _VEC_2_H__

#include "pch.h"

#ifdef EXPANSIONMATH_EXPORTS
#define EXPANSIONMATH_API __declspec(dllexport)
#else
#define EXPANSIONMATH_API __declspec(dllimport)
#endif

#include <valarray>

class EXPANSIONMATH_API vec2f {
public:
    vec2f(float x, float y);
    ~vec2f();
    
    float getX();
    void setX(float x);
    
    float getY();
    void setY(float y);
    
    void setAll(float x, float y);
    
    float dotProduct(vec2f const& a);
    
    bool operator==(vec2f const& a);
    bool operator!=(vec2f const& a);
    vec2f operator+(vec2f const& a);
    vec2f operator-(vec2f const& a);
    
private:
    float m_x;
    float m_y;
};

#endif
