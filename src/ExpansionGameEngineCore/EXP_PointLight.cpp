#include "pch.h"
#include "EXP_PointLight.h"

EXP_PointLight::EXP_PointLight(EXP_Game* gameinstance, vec3f pos, vec3f color, float brightness, float radius) : EXP_Component(pos, vec3f(), vec3f()), RD_PointLight(pos, color, brightness, radius) {
	m_gameinstance = gameinstance;

	vec4f glpos(pos, 1);
	glpos = m_parent_mat * glpos;
	RD_PointLight::SetPosition(glpos.XYZ());
	EXP_Component::m_pos = glpos.XYZ();

	RegisterLightToGame();
}
EXP_PointLight::~EXP_PointLight() {

}

void EXP_PointLight::RegisterLightToGame() {
	m_gameinstance->RegisterPointLight(this);
}

void EXP_PointLight::SetPosition(vec3f nPos) {
	vec4f glpos(nPos, 1);
	glpos = m_parent_mat * glpos;

	RD_PointLight::SetPosition(glpos.XYZ());
	EXP_Component::m_pos = nPos;
}

vec3f EXP_PointLight::GetPosition() {
	return RD_PointLight::GetPosition();
}

void EXP_PointLight::UseParentMatrix(mat4f mat) {
	m_parent_mat = mat;

	vec4f glpos(EXP_Component::m_pos, 1);
	glpos = m_parent_mat * glpos;

	RD_PointLight::SetPosition(glpos.XYZ());
}