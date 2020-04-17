#include "pch.h"
#include "EXP_Camera.h"

EXP_Camera::EXP_Camera(EXP_Game* gameinstance, vec3f pos, vec3f rot, vec3f scale, vec3f YawPitchRoll, float FOV, float Near, float Far) : EXP_Component(pos, rot, scale), RD_Camera(gameinstance->GetRenderer(), FOV, Near, Far, pos, YawPitchRoll) {

	m_gameinstance = gameinstance;

}

EXP_Camera::~EXP_Camera() {

}

void EXP_Camera::Use() {
	UseCamera();
	m_gameinstance->RegisterCamera(this);
}

void EXP_Camera::Translate(vec3f translation, bool changeSubject) {
	TranslateCamera(translation, changeSubject);
	SetPosition(GetLocation());
}


//---------------------------------------------------------------------------------

vec3f EXP_Camera::GetPosition() {
	return RD_Camera::GetLocation();
}

vec3f EXP_Camera::GetRotation() {
	return RD_Camera::GetYPR();
}

vec3f EXP_Camera::GetScale() {
	return vec3f();
}

void EXP_Camera::SetPosition(vec3f nPos) {
	RD_Camera::SetLocation(nPos);
}

void EXP_Camera::SetRotation(vec3f YPR) {
	RD_Camera::SetYPR(YPR);
}