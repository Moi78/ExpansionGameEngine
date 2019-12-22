#include "pch.h"
#include "EXP_PointLight.h"

EXP_PointLight::EXP_PointLight(EXP_Game* gameinstance, vec3f pos, vec3f rot, vec3f scale, vec3f color, float brightness) : EXP_Component(pos, rot, scale) {
	m_gameinstance = gameinstance;
	m_light = new RD_PointLight(pos, color, brightness);

	RegisterLightToGame();
}
EXP_PointLight::~EXP_PointLight() {
	delete m_light;
}

void EXP_PointLight::RegisterLightToGame() {
	if (m_light) {
		m_gameinstance->RegisterPointLight(m_light);
	}
	else {
		std::cerr << "Cannot create and register light" << std::endl;
	}
}