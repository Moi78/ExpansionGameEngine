#ifndef _VEC3_H__
#define _VEC3_H__

#include "pch.h"

#ifdef EXPANSIONMATH_EXPORTS
#define EXPANSIONMATH_API __declspec(dllexport)
#else
#define EXPANSIONMATH_API __declspec(dllimport)
#endif

#include <valarray>
#include <string>

class EXPANSIONMATH_API vec3f {
public:
    vec3f(float x = 0.0f, float y = 0.0f, float z = 0.0f);
    ~vec3f();
    
    float getX();
    float getY();
    float getZ();
    
    void setX(float x);
    void setY(float y);
    void setZ(float z);
    
    void setAll(float x, float y, float z);
    
    float dotProduct(vec3f a);
    
    std::string ConvertToParserFormat(std::string prefix);
    
    bool operator==(vec3f const& a);
    bool operator!=(vec3f const& a);
    
    vec3f operator+(vec3f const& a);
    vec3f operator-(vec3f const& a);

private:
    float m_x;
    float m_y;
    float m_z;
};

class EXPANSIONMATH_API vec3i {
public:
    vec3i(int x, int y, int z);
    ~vec3i();
    
    int getX();
    int getY();
    int getZ();
    
    void setX(int x);
    void setY(int y);
    void setZ(int z);
    
    void setAll(int x, int y, int z);
    
    float dotProduct(vec3i a);
    
    bool operator==(vec3i const& a);
    bool operator!=(vec3i const& a);
    
    vec3i operator+(vec3i const& a);
    vec3i operator-(vec3i const& a);
    
private:
    int m_x, m_y, m_z;
};

class EXPANSIONMATH_API vec3d {
public:
    vec3d(double x, double y, double z);
    ~vec3d();
    
    double getX();
    double getY();
    double getZ();
    
    void setX(double x);
    void setY(double y);
    void setZ(double z);
    
    void setAll(double x, double y, double z);
    
    float dotProduct(vec3d a);
    
    bool operator==(vec3d const& a);
    bool operator!=(vec3d const& a);
    
    vec3d operator+(vec3d const& a);
    vec3d operator-(vec3d const& a);
    
private:
    double m_x, m_y, m_z;
};

#endif //_VEC3_H__
