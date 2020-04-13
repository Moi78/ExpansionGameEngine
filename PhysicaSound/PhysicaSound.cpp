#include "pch.h"

#include "PhysicaSound.h"

#include <iostream>

PSound::PSound()
{
    Devices = {};
}

PSound::~PSound()
{
}

bool PSound::initAL()
{
    device = alcOpenDevice(NULL);
    
    if(device == NULL) {
        std::cerr << "Could not create device." << std::endl;
        return false;
    }
    
    context = alcCreateContext(device, NULL);
    
    if(context == NULL) {
        std::cerr << "Could not create context." << std::endl;
        return false;
    }
    
    if(alcMakeContextCurrent(context) == false) {
        std::cerr << "Could not make context current." << std::endl;
        return false;
    }
    
    initialized = true;
    
    return true;
}


bool PSound::playSimpleSound(std::string wavFile, float gain)
{
    if(!initialized)
        return false;
    
    SF_INFO FileInfo;
    SNDFILE* File = sf_open(wavFile.c_str(), SFM_READ, &FileInfo);
    
    if(File == NULL) {
        std::cerr << "Could not open file " << wavFile << "." << std::endl;
        return false;
    }
    
    ALsizei NBsamples = static_cast<ALsizei>(FileInfo.channels * FileInfo.frames);
    ALsizei SampleRate = static_cast<ALsizei>(FileInfo.samplerate);
    
    std::vector<ALshort> Samples(NBsamples);
    if(sf_read_short(File, &Samples[0], NBsamples) < NBsamples) {
        std::cerr << "Error on sf_read_short()." << std::endl;
        return false;
    }
    sf_close(File);
    
    ALenum format;
    
    switch(FileInfo.channels) {
        case 1: format = AL_FORMAT_MONO16; break;
        case 2: format = AL_FORMAT_STEREO16; break;
        default: std::cerr << "Could not find audio format." << std::endl; return false;
    }
    
    ALuint Buffer;
    alGenBuffers(1, &Buffer);
    
    alBufferData(Buffer, format, &Samples[0], NBsamples * sizeof(ALushort), SampleRate);
    
    if(alGetError() != AL_NO_ERROR) {
        std::cerr << "Error with AL." << std::endl;
        return false;
    }
    
    ALuint source;
    alGenSources(1, &source);
    
    alSourcei(source, AL_BUFFER, Buffer);
    alSourcef(source, AL_GAIN, gain);
    
    alSourcePlay(source);
    //ALint status;
    ALint state;
    do {
        alGetSourcei(source, AL_SOURCE_STATE, &state);

        std::this_thread::sleep_for(std::chrono::seconds(1));
    } while (state == AL_PLAYING);
    
    alDeleteBuffers(1, &Buffer);
    
    alSourcei(source, AL_BUFFER, 0);
    alDeleteSources(1, &source);
    
    return true;
}

void PSound::GetDevice()
{
    const ALchar* DeviceList = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
    
    if(DeviceList) {
        while(strlen(DeviceList) > 0) {
            Devices.push_back(DeviceList);
            DeviceList += strlen(DeviceList) + 1;
        }    
    }
}

void PSound::shutdownAL()
{
    std::cout << "Shutting down AL" << std::endl;
    
    context = alcGetCurrentContext();
    
    device = alcGetContextsDevice(context);
    
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    
    alcCloseDevice(device);
}

bool PSound::mainLoop()
{
    if(!initialized) {
        std::cerr << "AL not initialized" << std::endl;
        return false;
    }
    
    if(m_listener == NULL) {
        std::cerr << "No listener defined." << std::endl;
        return false;
    }
    
    ALfloat listener_pos[3] = {m_listener->getPosition().getX(), m_listener->getPosition().getY(), m_listener->getPosition().getZ()};
    alListenerfv(AL_POSITION, listener_pos);

    ALfloat list_ori[3] = { m_listener->getOrientation().getX() * -1, m_listener->getOrientation().getY() * -1, m_listener->getOrientation().getZ() };
    alListenerfv(AL_ORIENTATION, list_ori);
    
    for(int i = 0; i < m_emitters.size(); i++) {
        m_emitters[i]->refreshEmitterState();
        
        PS_Emitter* emitter_buffer = m_emitters[i].get();
        PS_Emitter* emitter_backup_buffer = m_emitters_backup[i].get();
        
        for(int o = 0; o < m_volumes.size(); o++) {
            m_volumes[o]->refreshListener(m_listener, emitter_buffer, emitter_backup_buffer);
        }
    }

    return true;
}

void PSound::RegisterListener(PS_Listener* listener)
{
    m_listener = listener;
}

void PSound::RegisterEmitter(PS_Emitter* emitter, bool directPlay)
{
    m_emitters.push_back(std::make_shared<PS_Emitter>(emitter));
    m_emitters_backup.push_back(std::make_shared<PS_Emitter>(emitter));
    std::cout << "Registered " << emitter->getFile() << "." << std::endl;
    
    if(directPlay)
        emitter->Play();
}

void PSound::RegisterVolume(PS_SoundVolume* volume)
{
    m_volumes.push_back(std::make_shared<PS_SoundVolume>(volume));
}

bool PSound::playSound3D(std::string wavFile, vec3f position, float gain)
{
    if(!initialized)
        return false;
    
    alGetError();
    
    SF_INFO FileInfo;
    SNDFILE* File = sf_open(wavFile.c_str(), SFM_READ, &FileInfo);
    
    if(File == NULL) {
        std::cerr << "Could not open file " << wavFile << "." << std::endl;
        return false;
    }
    
    ALsizei NBsamples = static_cast<ALsizei>(FileInfo.channels * FileInfo.frames);
    ALsizei SampleRate = static_cast<ALsizei>(FileInfo.samplerate);
    
    std::vector<ALshort> Samples(NBsamples);
    if(sf_read_short(File, &Samples[0], NBsamples) < NBsamples) {
        std::cerr << "Error on sf_read_short()." << std::endl;
        return false;
    }
    sf_close(File);
    
    ALenum format = AL_FORMAT_MONO16;
    
    ALuint Buffer;
    alGenBuffers(1, &Buffer);
    
    alBufferData(Buffer, format, &Samples[0], NBsamples * sizeof(ALushort), SampleRate);
    
    if(alGetError() != AL_NO_ERROR) {
        std::cerr << "Error with AL." << std::endl;
        return false;
    }
    
    ALuint source;
    alGenSources(1, &source);

    alSource3f(source, AL_POSITION, position.getX(), position.getY(), position.getZ());
    
    //DEBUG
    std::cout << "Source : " << source << " " << position.getX() << " " << position.getY() << " " << position.getZ() << std::endl;
    
    alSource3f(source, AL_VELOCITY, 0.0, 0.0, 0.0);
    alSourcef(source, AL_GAIN, gain);
    alSourcef(source, AL_PITCH, 2);

    alSourcei(source, AL_BUFFER, Buffer);

    alSourcePlay(source);
    //ALint status;
    ALint state;
    do {
        alGetSourcei(source, AL_SOURCE_STATE, &state);

        std::this_thread::sleep_for(std::chrono::seconds(1));
    } while (state == AL_PLAYING);
    
    alDeleteBuffers(1, &Buffer);
    
    alSourcei(source, AL_BUFFER, 0);
    alDeleteSources(1, &source);
    
    return true;
}
