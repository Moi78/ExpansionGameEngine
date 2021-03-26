#include "pch.h"
#include "EXP_Particles.h"

EXP_SMParticleEmitter::EXP_SMParticleEmitter(
	EXP_Game* game, RD_ShaderMaterial* mat,
	vec3f pos, vec3f rot, vec3f scale,
	vec3f dir, float rate, float velocity, float lifetime
) :
	EXP_Component(pos, rot, scale),
	RD_SmallParticleEmitter(game->GetRenderer(), mat, pos, dir, rate, velocity, lifetime)
{
	game->GetRenderer()->RegisterParticleEmitter(this);
}

EXP_SMParticleEmitter::~EXP_SMParticleEmitter() {

}

void EXP_SMParticleEmitter::UseParentMatrix(mat4f parent) {
	RD_SmallParticleEmitter::UseParentMatrix(parent);
	EXP_Component::m_parent_mat = parent;
}