#pragma once

#ifdef _WIN32
	#ifdef EXPANSIONGAMEENGINECORE_EXPORTS
		#define EXPGE_API __declspec(dllexport)
	#else
		#define EXPGE_API __declspec(dllimport)
	#endif //EXPANSIONGAMEENGINECORE_EXPORTS
#else
	#define EXPGE_API
#endif //_WIN32

#include <RD_Particles.h>

#include "EXP_Game.h"
#include "EXP_Component.h"

class EXPGE_API EXP_SMParticleEmitter :
	public EXP_Component,
	public RD_SmallParticleEmitter
{
public:
	EXP_SMParticleEmitter(EXP_Game* game, RD_ShaderMaterial* mat, vec3f pos, vec3f rot, vec3f scale, vec3f dir, float rate, float velocity, float lifetime);
	virtual ~EXP_SMParticleEmitter();

	virtual void UseParentMatrix(mat4f parent);
};