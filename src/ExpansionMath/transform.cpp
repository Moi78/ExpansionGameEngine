#include "pch.h"

#include "transform.h"

vec3f Transform::RotatePointZ(vec3f point, vec3f pointOrigin, float angle)
{
    float angleD = angle*PI/180;
    
    float c = cos(angleD);
    float s = sin(angleD);
    
    vec3f locPoint = point - pointOrigin;
    
    float nx = locPoint.getX()*c - locPoint.getY()*s;
    float ny = locPoint.getY()*c + locPoint.getX()*s;
    
    vec3f finalPoint(nx, ny, locPoint.getZ());
    finalPoint = finalPoint + pointOrigin;
    
    std::cout << nx << " " << ny << " " << finalPoint.getZ() << std::endl;
    
    return finalPoint;
}

vec3f Transform::RotatePointY(vec3f point, vec3f pointOrigin, float angle)
{
    float angleD = angle*PI/180;
    
    float c = cos(angleD);
    float s = sin(angleD);
    
    vec3f locPoint = point - pointOrigin;
    
    float nx = locPoint.getX()*c - locPoint.getZ()*s;
    float nz = locPoint.getZ()*c + locPoint.getX()*s;
    
    vec3f finalPoint(nx, locPoint.getY(), nz);
    finalPoint = finalPoint + pointOrigin;
    
    std::cout << nx << " " << finalPoint.getY() << " " << nz << std::endl;
    
    return finalPoint;
}

vec3f Transform::RotatePointX(vec3f point, vec3f pointOrigin, float angle)
{
    float angleD = angle*PI/180;
    
    float c = cos(angleD);
    float s = sin(angleD);
    
    vec3f locPoint = point - pointOrigin;
    
    float ny = locPoint.getY()*c - locPoint.getZ()*s;
    float nz = locPoint.getZ()*c + locPoint.getY()*s;
    
    vec3f finalPoint(locPoint.getX(), ny, nz);
    finalPoint = finalPoint + pointOrigin;
    
    std::cout << finalPoint.getX() << " " << ny << " " << nz << std::endl;
    
    return finalPoint;
}
