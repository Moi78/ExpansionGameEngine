#ifndef _PS_LISTENER_H__
#define _PS_LISTENER_H__

#include "pch.h"

#ifdef _WIN32
#ifdef PHYSICASOUND_EXPORTS
#define PHYSICASOUND_API __declspec(dllexport)
#else
#define PHYSICASOUND_API __declspec(dllimport)
#endif

#else

#define PHYSICASOUND_API

#endif

#include <vec3.h>

class PHYSICASOUND_API PS_Listener {
public:
    PS_Listener(vec3f lstnr_pos, vec3f orientation);
    
    void setPosition(vec3f n_pos);
    void setOrientation(vec3f n_orientation);
    
    vec3f getPosition();
    vec3f getOrientation();
    
private:
    vec3f m_pos;
    vec3f m_ori;
    
    int m_volume;
    int m_pitch;
};

#endif
