#ifndef _PS_LISTENER_H__
#define _PS_LISTENER_H__

#include "pch.h"

#ifdef PHYSICASOUND_EXPORTS
#define PHYSICASOUND_API __declspec(dllexport)
#else
#define PHYSICASOUND_API __declspec(dllimport)
#endif

#include <vec3.h>

class PHYSICASOUND_API PS_Listener {
public:
    PS_Listener(vec3f lstnr_pos);
    
    void setPosition(vec3f n_pos);
    
    vec3f getPosition();
    
private:
    vec3f m_pos;
    
    int m_volume;
    int m_pitch;
};

#endif
