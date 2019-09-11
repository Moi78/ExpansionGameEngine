#include "pch.h"

#include "PS_SoundVolume.h"

PS_SoundVolume::PS_SoundVolume(BoxVol volume) : m_volume(volume) {
    m_OverridesArray.push_back(m_volumeOverride);
    m_OverridesArray.push_back(m_pitchOverride);
}

PS_SoundVolume::~PS_SoundVolume()
{
}

vec3f PS_SoundVolume::getVolumePosition()
{
    return m_volume.getWorldPosition();
}

vec3f PS_SoundVolume::getVolumeSize()
{
    return m_volume.getSize();
}

void PS_SoundVolume::setVolumePosition(vec3f npos)
{
    m_volume.setWorldPosition(npos);
}

void PS_SoundVolume::setVolumeSize(vec3f nsize)
{
    m_volume.setSize(nsize);
}

float PS_SoundVolume::getOverride(int override) {
    return m_OverridesArray[override];
}

void PS_SoundVolume::setOverride(int override, float value)
{
    m_OverridesArray[override] = value;
}

void PS_SoundVolume::refreshListener(PS_Listener* lstnr, PS_Emitter* emitters, PS_Emitter* emittersBackup)
{
    vec3f lpos = lstnr->getPosition();

    if(pointInVolume(m_volume, lpos)) {
        alSourcef(emitters->getSource(), AL_GAIN, m_OverridesArray[PS_VOLUME_OVERRIDE]);
        alSourcef(emitters->getSource(), AL_PITCH, m_OverridesArray[PS_PITCH_OVERRIDE]);
    } else {
        emitters = emittersBackup;
        emitters->refreshSourceProperties();
    }
}
