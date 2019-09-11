#ifndef _PS_EMITTER_H__
#define _PS_EMITTER_H__

#include "pch.h"

#ifdef PHYSICASOUND_EXPORTS
#define PHYSICASOUND_API __declspec(dllexport)
#else
#define PHYSICASOUND_API __declspec(dllimport)
#endif

#include <iostream>
#include <string>
#include <vector>

#include <sndfile.h>

#include <AL/al.h>
#include <AL/alc.h>

#include <vec3.h>

class PHYSICASOUND_API PS_Emitter {
public:
    PS_Emitter(vec3f position, vec3f velocity, float volume, float pitch, bool looping = true);
    virtual ~PS_Emitter();
    
    bool loadAudioFile(std::string wavFile);
    bool bufferizeSound();
    
    virtual void Play();
    virtual void Pause();
    virtual void Stop();
    
    void setVolume(float volume);
    vec3f setVelocity(vec3f velocity);
    void setPitch(float pitch);
    void setLooping(bool looping);
    
    float getVolume();
    vec3f getVelocity();
    float getPitch();
    ALint getEmitterState();
    void refreshEmitterState();
    void refreshSourceProperties();
    ALuint getSource();
    
    SF_INFO getFileInfo();
    SNDFILE* getFile();
    
    ALsizei getNumberOfSamples();
    ALsizei getSampleRate();
    
    bool isPlaying();
    
private:
    bool allInit = false;
    
    //Common attribute
    float m_volume;
    float m_pitch;
    
    vec3f m_position;
    vec3f m_velocity;
    
    bool m_looping;
    
    //File info
    SF_INFO m_FileInfo;
    SNDFILE* m_File;
    
    //AL variables
    ALsizei m_nb_samples;
    ALsizei m_sample_rate;
    std::vector<ALshort> m_samples;
    ALuint m_format = AL_FORMAT_MONO16;
    ALuint m_buffer;
    ALuint m_source;
    ALint m_state;
};

#endif
