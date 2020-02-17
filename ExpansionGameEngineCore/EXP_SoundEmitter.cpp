#include "pch.h"
#include "EXP_SoundEmitter.h"

EXP_SoundEmitter::EXP_SoundEmitter(EXP_Game* gameinstance, vec3f pos, vec3f velocity, float gain, float pitch, bool loop, std::string sound_ref) : PS_Emitter(pos, velocity, gain, pitch, loop) {

	std::string fullref = gameinstance->GetGameInfo().RootGameContentFolder + sound_ref;

	loadAudioFile(fullref);
	bufferizeSound();

	gameinstance->RegisterSoundEmitter(this);
}

EXP_SoundEmitter::~EXP_SoundEmitter() {

}