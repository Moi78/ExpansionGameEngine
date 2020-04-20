#include "pch.h"
#include "EXP_PointLight.h"

EXP_PointLight::EXP_PointLight(EXP_Game* gameinstance, vec3f pos, vec3f color, float brightness) : EXP_Component(pos, vec3f(), vec3f()), RD_PointLight(pos, color, brightness) {
	m_gameinstance = gameinstance;

	RegisterLightToGame();
}
EXP_PointLight::~EXP_PointLight() {

}

void EXP_PointLight::RegisterLightToGame() {
	m_gameinstance->RegisterPointLight(this);
}

void EXP_PointLight::SetPosition(vec3f nPos) {
	RD_PointLight::SetPosition(nPos);
	EXP_Component::m_pos = nPos;
}

vec3f EXP_PointLight::GetPosition() {
	return RD_PointLight::GetPosition();
}