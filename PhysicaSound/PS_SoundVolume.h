#ifndef _PS_SOUND_VOLUME_H__
#define _PS_SOUND_VOLUME_H__

#include "pch.h"

#ifdef PHYSICASOUND_EXPORTS
#define PHYSICASOUND_API __declspec(dllexport)
#else
#define PHYSICASOUND_API __declspec(dllimport)
#endif

#define PS_VOLUME_OVERRIDE 0
#define PS_PITCH_OVERRIDE 1

#include <vec3.h>
#include <BoxVol.h>
#include <volumes.h>

#include "PS_Listener.h"
#include "PS_Emitter.h"

#include <AL/al.h>
#include <AL/alc.h>

#include <iostream>
#include <vector>

class PHYSICASOUND_API PS_SoundVolume {
public:
    PS_SoundVolume(BoxVol volume);
    ~PS_SoundVolume();
    
    vec3f getVolumePosition();
    void setVolumePosition(vec3f npos);
    
    vec3f getVolumeSize();
    void setVolumeSize(vec3f nsize);
    
    float getOverride(int override);
    void setOverride(int override ,float value);
    
    void refreshListener(PS_Listener* lstnr, PS_Emitter* emitters, PS_Emitter* emittersBackup);
    
private:
    //Common values
    BoxVol m_volume;
    
    //Overrides
    float m_volumeOverride = 1.0;
    float m_pitchOverride = 1.0;
    
    std::vector<float> m_OverridesArray;
};

#endif
