#ifndef _PHYSICASOUND_H__
#define _PHYSICASOUND_H__

#include "pch.h"

#ifdef PHYSICASOUND_EXPORTS
#define PHYSICASOUND_API __declspec(dllexport)
#else
#define PHYSICASOUND_API __declspec(dllimport)
#endif

#include <AL/al.h>
#include <AL/alc.h>

#include <sndfile.h>

#include <cstring>
#include <string>
#include <vector>

#include "PS_Listener.h"
#include "PS_Emitter.h"
#include "PS_SoundVolume.h"

class PHYSICASOUND_API PSound {
public:
    PSound();
    ~PSound();
    
    bool playSimpleSound(std::string wavFile);
    bool playSound3D(std::string wavFile, vec3f position);
    
    bool initAL();
    void GetDevice();
    void shutdownAL();
    
    void RegisterListener(PS_Listener* listener);
    void RegisterEmitter(PS_Emitter* emitter, bool directPlay);
    void RegisterVolume(PS_SoundVolume* volume);
    
    bool mainLoop();
    
private:
    ALCcontext* context;
    ALCdevice* device;
    
    bool initialized = false;
    std::vector<std::string> Devices;
    
    PS_Listener* m_listener = NULL;
    std::vector<PS_Emitter*> m_emitters;
    std::vector<PS_SoundVolume*> m_volumes;
    
    std::vector<PS_Emitter*> m_emitters_backup;
};

#endif
