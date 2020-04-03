#pragma once

#ifndef _EXP_SOUND_EMITTER_H__
#define _EXP_SOUND_EMITTER_H__

#ifdef _WIN32

#ifdef EXPANSIONGAMEENGINECORE_EXPORTS
#define EXPGE_API __declspec(dllexport)
#else
#define EXPGE_API __declspec(dllimport)
#endif

#else

#define EXPGE_API

#endif //_WIN32

#include <PS_Emitter.h>

#include "EXP_Game.h"

class EXPGE_API EXP_SoundEmitter : public PS_Emitter
{
public:
	EXP_SoundEmitter(EXP_Game* gameinstance, vec3f pos, vec3f velocity, float gain, float pitch, bool loop, std::string sound_ref);
	~EXP_SoundEmitter();
};

#endif //_EXP_SOUND_EMITTER_H__