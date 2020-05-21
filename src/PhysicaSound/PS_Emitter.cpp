#include "pch.h"

#include "PS_Emitter.h"

PS_Emitter::~PS_Emitter()
{
    alDeleteBuffers(1, &m_buffer);
    alSourcei(m_source, AL_BUFFER, 0);
    alDeleteSources(1, &m_source);

    m_samples.clear();

    delete m_File;
}

PS_Emitter::PS_Emitter(vec3f position, vec3f velocity, float volume, float pitch, bool looping) : m_volume(volume), m_pitch(pitch), m_position(position), m_velocity(velocity), m_looping(looping) {
    
}

bool PS_Emitter::loadAudioFile(std::string wavFile)
{
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
    
    m_samples = Samples;
    
    sf_close(File);
    
    m_File = File;
    m_FileInfo = FileInfo;
    
    m_nb_samples = NBsamples;
    m_sample_rate = SampleRate;
    
    allInit = true;
    
    return true;
}

void PS_Emitter::setVolume(float volume)
{
    m_volume = volume;
}

void PS_Emitter::setPitch(float pitch)
{
    m_pitch = pitch;
}

vec3f PS_Emitter::setVelocity(vec3f velocity)
{
    return m_velocity = velocity;
}

SF_INFO PS_Emitter::getFileInfo()
{
    return m_FileInfo;
}

ALsizei PS_Emitter::getNumberOfSamples()
{
    return m_nb_samples;
}

float PS_Emitter::getPitch()
{
    return m_pitch;
}

ALsizei PS_Emitter::getSampleRate()
{
    return m_sample_rate;
}

vec3f PS_Emitter::getVelocity()
{
    return m_velocity;
}

float PS_Emitter::getVolume()
{
    return m_volume;
}

void PS_Emitter::Pause()
{
    alSourcePause(m_source);
}

void PS_Emitter::Play()
{
    alSourcePlay(m_source);
}

void PS_Emitter::Stop()
{
    alSourceStop(m_source);
}

SNDFILE * PS_Emitter::getFile()
{
    return m_File;
}

bool PS_Emitter::bufferizeSound() {
    
    alGenBuffers(1, &m_buffer);
    alBufferData(m_buffer, m_format, &m_samples[0], m_nb_samples * sizeof(ALushort), m_sample_rate);
    
    if(alGetError() != AL_NO_ERROR) {
        std::cerr << "Error with AL" << std::endl;
        return false;
    }
    
    alGenSources(1, &m_source);
    alSourcei(m_source, AL_BUFFER, m_buffer);
    
    if(m_looping)
        alSourcei(m_source, AL_LOOPING, 1);
    else
        alSourcei(m_source, AL_LOOPING, 0);
    
    alSourcef(m_source, AL_GAIN, m_volume);
    alSourcef(m_source, AL_PITCH, m_pitch);
    
    alSource3f(m_source, AL_VELOCITY, m_velocity.getX(), m_velocity.getY(), m_velocity.getZ());
    alSource3f(m_source, AL_POSITION, m_position.getX(), m_position.getY(), m_position.getZ());
    
    if(alGetError() != AL_NO_ERROR) {
        std::cerr << "Error with AL" << std::endl;
        return false;
    }
    
    return true;
}

ALint PS_Emitter::getEmitterState()
{
    alGetSourcei(m_source, AL_SOURCE_STATE, &m_state);
    
    return m_state;
}

bool PS_Emitter::isPlaying()
{
    if(m_state == AL_PLAYING)
        return true;
    
    return false;
}

void PS_Emitter::refreshEmitterState()
{
    alGetSourcei(m_source, AL_SOURCE_STATE, &m_state);
}

void PS_Emitter::refreshSourceProperties()
{
    alSourcef(m_source, AL_GAIN, m_volume);
    alSourcef(m_source, AL_PITCH, m_pitch);
}

void PS_Emitter::setLooping(bool looping)
{
    m_looping = looping;
    
    if(m_looping)
        alSourcei(m_source, AL_LOOPING, 1);
    else
        alSourcei(m_source, AL_LOOPING, 0);
}

ALuint PS_Emitter::getSource()
{
    return m_source;
}

vec3f PS_Emitter::getLocation() {
    return m_position;
}