#include "pch.h"
#include "RD_Camera.h"

RD_Camera::RD_Camera(RaindropRenderer* rndr, float FOVinDegrees, float CamNear, float CamFar, vec3f position, vec3f YawPitchRoll, bool noInitUpdt) : view(1.0f), projection(1.0f) {
	m_rndr = rndr; //Reference to renderer;

	m_pos = position; //Camera position
	m_subject = vec3f(); //Camera center of view
	m_up = vec3f(0.0, 0.0, 1.0); //Up default
	m_yawPitchRoll = YawPitchRoll;

	ComputeYPR();

	FOV = FOVinDegrees;
	m_near = CamNear;
	m_far = CamFar;

	if(!noInitUpdt)
		UpdateCamera();
}

RD_Camera::~RD_Camera() {

}

void RD_Camera::UpdateCamera() {
	UpdateView();
	UpdateProj();
}

void RD_Camera::UpdateView() {
	view = LookAt(m_pos, m_pos + m_subject, m_up); //View matrix

	m_rndr->PushViewMatrix(view);
}

void RD_Camera::UpdateProj() {
	vec2f vp_scale = m_rndr->GetViewportScale();

	projection = ProjPersp<float>(
		DEG_TO_RAD(FOV),
		(float)(m_rndr->GetViewportSize().getX() * vp_scale.getX()) / (m_rndr->GetViewportSize().getY() * vp_scale.getY()),
		m_near, m_far
	);

	m_rndr->PushProjMatrix(projection);
}

void RD_Camera::UseCamera(RD_ShaderLoader* shader) {
	m_rndr->PushProjMatrix(projection);
	m_rndr->PushViewMatrix(view);

	m_rndr->PushCamPos(m_pos);
}

void RD_Camera::SetLocation(vec3f position) {
	m_pos = position;
	UpdateView();

	m_rndr->PushCamPos(m_pos);
}

void RD_Camera::SetSubject(vec3f lookingAt) {
	m_subject = lookingAt;
	UpdateView();
}

void RD_Camera::SetFOV(float FOVinDegrees) {
	FOV = FOVinDegrees;
	UpdateProj();
}

void RD_Camera::RotateCamera(vec3f rotation) {
	mat4f rot(1.0f);
	rot = RotateMatrix(rot, rotation);

	m_subject = vec4f(rot * m_subject).XYZ();

	UpdateView();
}

void RD_Camera::TranslateCamera(vec3f translation, bool changeSub) {
	m_pos = m_pos + translation;

	if (changeSub) {
		m_subject = m_subject + translation;
	}

	UpdateView();
}

vec3f RD_Camera::GetSubject() {
	return m_subject;
}

vec3f RD_Camera::GetLocation() {
	return m_pos;
}

vec3f RD_Camera::GetForwardVector() {
	vec3f fwd = m_subject;
	fwd.NormalizeVector();

	return fwd;
}

vec3f RD_Camera::GetRightVector() {
	vec3f forwardVector = GetForwardVector();

	vec3f rightVec = Cross(vec3f(0.0f, 0.0f, 1.0f), forwardVector);

	return rightVec;
}

void RD_Camera::AddPitch(float pitch) {
	m_yawPitchRoll = m_yawPitchRoll + vec3f(0.0f, pitch, 0.0f);
	//SAAAAAAAAALEEEEEEEE
	m_yawPitchRoll.setY(std::clamp<float>(m_yawPitchRoll.getY(), -89.9999f, 89.9999f));

	ComputeYPR();
}

void RD_Camera::AddYaw(float yaw) {
	m_yawPitchRoll = m_yawPitchRoll + vec3f(yaw, 0.0f, 0.0f);

	ComputeYPR();
}

void RD_Camera::AddRoll(float roll) {
	m_yawPitchRoll = m_yawPitchRoll + vec3f(0.0f, 0.0f, roll);

	ComputeYPR();
}

void RD_Camera::ComputeYPR() {
	const float yaw = DEG_TO_RAD(m_yawPitchRoll.getX());
	const float pitch = DEG_TO_RAD(m_yawPitchRoll.getY());

	const float roll2 = DEG_TO_RAD(m_yawPitchRoll.getZ()) / 2;
	
	m_subject.setAll(
		(float)cos((double)yaw) * (float)cos((double)pitch),
		(float)sin((double)yaw) * (float)cos((double)pitch),
		(float)sin((double)pitch)
	);

	//Cam rot around fwd vec
	vec3f fwd = GetForwardVector();
	const float sRoll2 = sin(roll2);

	Quat rotator(cos(roll2), sin(roll2) * fwd.getX(), sin(roll2) * fwd.getY(), sin(roll2) * fwd.getZ());
	const Quat rotatorConj = rotator.GetConjugate();
	
	const Quat vector(0.0f, 0, 0, 1);

	Quat up_rot = (rotator * vector) * rotatorConj;
	m_up = up_rot.GetImagPart();

	UpdateView();
}

vec3f RD_Camera::GetYPR() {
	return m_yawPitchRoll;
}

void RD_Camera::SetYPR(vec3f YPR) {
	m_yawPitchRoll = YPR;

	ComputeYPR();
}