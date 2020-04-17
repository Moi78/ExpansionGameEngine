#include "pch.h"
#include "RD_Camera.h"

RD_Camera::RD_Camera(RaindropRenderer* rndr, float FOVinDegrees, float CamNear, float CamFar, vec3f position, vec3f YawPitchRoll) {
	m_rndr = rndr; //Attach to renderer;

	m_pos = position; //Camera position
	m_subject = vec3f(); //Camera center of view
	m_yawPitchRoll = YawPitchRoll;

	m_subject.setX(cos(glm::radians(YawPitchRoll.getX())) * cos(glm::radians(YawPitchRoll.getY())));
	m_subject.setZ(sin(glm::radians(YawPitchRoll.getY())));
	m_subject.setY(sin(glm::radians(YawPitchRoll.getX())) * cos(glm::radians(YawPitchRoll.getY())));

	FOV = FOVinDegrees;
	m_near = CamNear;
	m_far = CamFar;

	SetupCamera();
}

RD_Camera::~RD_Camera() {

}

void RD_Camera::SetupCamera() {
	projection = glm::perspective(glm::radians(FOV), (float)m_rndr->getWindowWidth() / m_rndr->getWindowHeigh(), m_near, m_far); //Projection matrix

	view = glm::lookAt(glm::vec3(m_pos.getX(), m_pos.getY(), m_pos.getZ()), glm::vec3(m_pos.getX(), m_pos.getY(), m_pos.getZ()) + glm::vec3(m_subject.getX(), m_subject.getY(), m_subject.getZ()), glm::vec3(0.0f, 0.0f, 1.0f)); //View matrix

	m_rndr->GetCurrentShader()->SetMatrix("projection", projection);
	m_rndr->GetCurrentShader()->SetMatrix("view", view);
	m_rndr->GetCurrentShader()->SetVec3("CamPos", m_pos);
}

void RD_Camera::UseCamera() {
	m_rndr->GetCurrentShader()->SetMatrix("projection", projection);
	m_rndr->GetCurrentShader()->SetMatrix("view", view);
	m_rndr->GetCurrentShader()->SetVec3("CamPos", m_pos);

	//std::cout << m_rndr->GetCurrentShader()->GetProgID() << std::endl;
}

void RD_Camera::UpdateCamera() {
	SetupCamera();
}

void RD_Camera::SetLocation(vec3f position) {
	m_pos = position;

	UpdateCamera();
}

void RD_Camera::SetSubject(vec3f lookingAt) {
	m_subject = lookingAt;

	UpdateCamera();
}

void RD_Camera::SetFOV(float FOVinDegrees) {
	FOV = FOVinDegrees;
	projection = glm::perspective(glm::radians(FOV), (float)m_rndr->getWindowWidth() / m_rndr->getWindowHeigh(), m_near, m_far);

	UpdateCamera();
}

void RD_Camera::RotateCamera(vec3f rotation) {
	glm::vec3 sub = glm::vec3(m_subject.getX(), m_subject.getY(), m_subject.getZ());
	sub = glm::rotateX(sub, rotation.getX());
	sub = glm::rotateY(sub, rotation.getY());
	sub = glm::rotateZ(sub, rotation.getZ());

	m_subject.setX(sub.x);
	m_subject.setY(sub.y);
	m_subject.setZ(sub.z);

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

vec3f RD_Camera::GetForwardVector() {
	glm::vec3 camTarg = glm::vec3(m_subject.getX(), m_subject.getY(), m_subject.getZ());

	glm::vec3 fwd = glm::normalize(glm::vec3(m_pos.getX(), m_pos.getY(), m_pos.getZ()) - camTarg);

	return vec3f(fwd.x, fwd.y, fwd.z);
}

vec3f RD_Camera::GetRightVector() {
	vec3f forwardVector = GetForwardVector();

	glm::vec3 rightVec = glm::normalize(glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(forwardVector.getX(), forwardVector.getY(), forwardVector.getZ())));

	return vec3f(rightVec.x, rightVec.y, rightVec.z);
}

void RD_Camera::AddPitch(float pitch) {
	m_yawPitchRoll = m_yawPitchRoll + vec3f(0.0f, pitch, 0.0f);

	ComputeYPR();
	UpdateCamera();
}

void RD_Camera::AddYaw(float yaw) {
	m_yawPitchRoll = m_yawPitchRoll + vec3f(yaw, 0.0f, 0.0f);

	ComputeYPR();
	UpdateCamera();
}

void RD_Camera::AddRoll(float roll) {
	m_yawPitchRoll = m_yawPitchRoll + vec3f(0.0f, 0.0f, roll);

	ComputeYPR();
	UpdateCamera();
}

void RD_Camera::ComputeYPR() {
	m_subject.setX(cos(glm::radians(m_yawPitchRoll.getX())) * cos(glm::radians(m_yawPitchRoll.getY())));
	m_subject.setZ(sin(glm::radians(m_yawPitchRoll.getY())));
	m_subject.setY(sin(glm::radians(m_yawPitchRoll.getX())) * cos(glm::radians(m_yawPitchRoll.getY())));
}

vec3f RD_Camera::GetYPR() {
	return m_yawPitchRoll;
}

void RD_Camera::SetYPR(vec3f YPR) {
	m_yawPitchRoll = YPR;

	ComputeYPR();
}