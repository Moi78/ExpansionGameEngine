#include "pch.h"
#include "EXP_Camera.h"

EXP_Camera::EXP_Camera(EXP_Game* gameinstance, vec3f pos, vec3f rot, vec3f scale, vec3f dir, float FOV, float Near, float Far) : EXP_Component(pos, rot, scale), RD_Camera(gameinstance->GetRenderer(), FOV, Near, Far, pos, dir) {

	m_gameinstance = gameinstance;

}

EXP_Camera::~EXP_Camera() {

}

void EXP_Camera::Use() {
	UseCamera();
	m_gameinstance->RegisterCamera(this);
}

void EXP_Camera::Translate(vec3f translation) {
	TranslateCamera(translation);
	SetPosition(GetLocation());
}