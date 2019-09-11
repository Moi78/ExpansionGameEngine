#ifndef _BOX_VOL_H__
#define _BOX_VOL_H__

#include "pch.h"

#ifdef EXPANSIONMATH_EXPORTS
#define EXPANSIONMATH_API __declspec(dllexport)
#else
#define EXPANSIONMATH_API __declspec(dllimport)
#endif

#include "vec3.h"

class EXPANSIONMATH_API BoxVol {
public:
    BoxVol(vec3f size, vec3f WorldPos);
    ~BoxVol();
    
    float getMinusX();
    float getX();
    
    float getMinusY();
    float getY();
    
    float getMinusZ();
    float getZ();
    
    vec3f getSize();
    vec3f getWorldPosition();
    
    void setSize(vec3f nsize);
    void setWorldPosition(vec3f npos);
private:
    vec3f m_size;
    vec3f m_WorldPos;
};

#endif
