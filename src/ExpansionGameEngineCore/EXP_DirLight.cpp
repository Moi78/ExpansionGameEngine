#include "pch.h"
#include "EXP_DirLight.h"

EXP_DirLight::EXP_DirLight(EXP_Game* game, vec3f dir, vec3f color, float brightness) : RD_DirLight(game->GetRenderer(), dir, color, brightness), EXP_Component(vec3f(), vec3f(), vec3f()) {
	game->GetRenderer()->AppendDirLight(this);
}

EXP_DirLight::~EXP_DirLight() {

}