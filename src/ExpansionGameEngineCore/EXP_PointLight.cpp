#include "pch.h"
#include "EXP_PointLight.h"

EXP_PointLight::EXP_PointLight(EXP_Game* gameinstance, vec3f pos, vec3f color, float brightness, float radius) : EXP_Component(pos, vec3f(), vec3f()), RD_PointLight(pos, color, brightness, radius) {
	m_gameinstance = gameinstance;

	glm::vec4 glpos(pos.getX(), pos.getY(), pos.getZ(), 1);
	glpos = glpos * m_parent_mat;
	RD_PointLight::SetPosition(vec3f(glpos.x, glpos.y, glpos.z));
	EXP_Component::m_pos = vec3f(glpos.x, glpos.y, glpos.z);

	RegisterLightToGame();
}
EXP_PointLight::~EXP_PointLight() {

}

void EXP_PointLight::RegisterLightToGame() {
	m_gameinstance->RegisterPointLight(this);
}

void EXP_PointLight::SetPosition(vec3f nPos) {
	glm::vec4 glpos(nPos.getX(), nPos.getY(), nPos.getZ(), 1);
	glpos = m_parent_mat * glpos;

	RD_PointLight::SetPosition(vec3f(glpos.x, glpos.y, glpos.z));
	EXP_Component::m_pos = nPos;
}

vec3f EXP_PointLight::GetPosition() {
	return RD_PointLight::GetPosition();
}

void EXP_PointLight::UseParentMatrix(glm::mat4 mat) {
	m_parent_mat = mat;

	glm::vec4 glpos(EXP_Component::m_pos.getX(), EXP_Component::m_pos.getY(), EXP_Component::m_pos.getZ(), 1);
	glpos = m_parent_mat * glpos;

	RD_PointLight::SetPosition(vec3f(glpos.x, glpos.y, glpos.z));
}