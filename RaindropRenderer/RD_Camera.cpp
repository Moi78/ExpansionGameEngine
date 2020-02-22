#include "pch.h"
#include "RD_Camera.h"

RD_Camera::RD_Camera(RaindropRenderer* rndr, float FOVinDegrees, float CamNear, float CamFar, vec3f position, vec3f lookingAt) {
	m_rndr = rndr; //Attach to renderer;

	m_pos = position; //Camera position
	m_subject = lookingAt; //Camera center of view

	FOV = FOVinDegrees;
	m_near = CamNear;
	m_far = CamFar;

	m_pitch = 0.0f;
	m_yaw = 0.0f;
	m_roll = 0.0f;

	SetupCamera();
}

RD_Camera::~RD_Camera() {

}

void RD_Camera::SetupCamera() {
	projection = glm::perspective(glm::radians(FOV), (float)m_rndr->getWindowWidth() / m_rndr->getWindowHeigh(), m_near, m_far); //Projection matrix

	view = glm::lookAt(glm::vec3(m_pos.getX(), m_pos.getY(), m_pos.getZ()), glm::vec3(m_subject.getX(), m_subject.getY(), m_subject.getZ()), glm::vec3(0.0f, 0.0f, 1.0f)); //View matrix

	m_rndr->GetCurrentShader()->SetMatrix("projection", projection);
	m_rndr->GetCurrentShader()->SetMatrix("view", view);
	m_rndr->GetCurrentShader()->SetVec3("CamPos", m_pos);
}

void RD_Camera::UseCamera() {
	m_rndr->GetCurrentShader()->SetMatrix("projection", projection);
	m_rndr->GetCurrentShader()->SetMatrix("view", view);
	m_rndr->GetCurrentShader()->SetVec3("CamPos", m_pos);
}

void RD_Camera::UpdateCamera() {
	SetupCamera();
	m_rndr->GetCurrentShader()->SetMatrix("projection", projection);
	m_rndr->GetCurrentShader()->SetMatrix("view", view);
	m_rndr->GetCurrentShader()->SetVec3("CamPos", m_pos);
}

void RD_Camera::SetLocation(vec3f position) {
	view = glm::lookAt(glm::vec3(position.getX(), position.getY(), position.getZ()), glm::vec3(m_subject.getX(), m_subject.getY(), m_subject.getZ()), glm::vec3(0.0f, 0.0f, 1.0f));

	UpdateCamera();
}

void RD_Camera::SetSubject(vec3f lookingAt) {
	view = glm::lookAt(glm::vec3(m_pos.getX(), m_pos.getY(), m_pos.getZ()), glm::vec3(lookingAt.getX(), lookingAt.getY(), lookingAt.getZ()), glm::vec3(0.0f, 0.0f, 1.0f));

	UpdateCamera();
}

void RD_Camera::SetFOV(float FOVinDegrees) {
	FOV = FOVinDegrees;
	projection = glm::perspective(glm::radians(FOV), (float)m_rndr->getWindowWidth() / m_rndr->getWindowHeigh(), m_near, m_far);

	UpdateCamera();
}

void RD_Camera::RotateCamera(vec3f rotation) {
	m_subject = m_subject + vec3f(sin(rotation.getX()), sin(rotation.getX()), sin(rotation.getY()));

	UpdateCamera();
}

void RD_Camera::TranslateCamera(vec3f translation, bool changeSub) {
	m_pos = m_pos + translation;

	if (changeSub) {
		m_subject = m_subject + translation;
	}

	UpdateCamera();
}

vec3f RD_Camera::GetSubject() {
	return m_subject;
}

vec3f RD_Camera::GetLocation() {
	return m_pos;
}