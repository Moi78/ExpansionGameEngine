#include "pch.h"
#include "EXP_Camera.h"

EXP_Camera::EXP_Camera(EXP_Game* gameinstance, vec3f pos, vec3f rot, vec3f scale, vec3f YawPitchRoll, float FOV, float Near, float Far, bool inheritParentRot) : EXP_Component(pos, rot, scale), RD_Camera(gameinstance->GetRenderer(), FOV, Near, Far, pos, YawPitchRoll) {

	m_gameinstance = gameinstance;
	m_inheritParentRot = inheritParentRot;

}

EXP_Camera::~EXP_Camera() {

}

void EXP_Camera::Use() {
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
	ComputeCamRealCoord();
	EXP_Component::m_pos = nPos;
}

void EXP_Camera::SetRotation(vec3f YPR) {
	RD_Camera::SetYPR(YPR);
	EXP_Component::m_rot = YPR;
}

void EXP_Camera::UseParentMatrix(mat4f mat) {
	m_parent_mat = mat;
	ComputeCamRealCoord();
}

void EXP_Camera::ComputeCamRealCoord() {
	mat4f trans(1.0f);
	mat4f rot(1.0f);
	mat4f scale(1.0f);

	trans = TranslateMatrix(trans, EXP_Component::m_pos);

	rot = RotateMatrix(rot, EXP_Component::m_rot);

	mat4f cam = trans * rot * scale;
	cam = m_parent_mat * cam;

	//Computing pos/rot
	vec4f pos(EXP_Component::m_pos, 1.0f);

	vec4f rpos = cam * pos;
	
	RD_Camera::SetLocation(rpos.XYZ());
	
	if (m_inheritParentRot) {
		/*glm::quat qrot = glm::toQuat(cam);
		glm::vec3 rrot = glm::eulerAngles(qrot);

		RD_Camera::SetYPR(vec3f(rrot.y, rrot.x, rrot.z));*/
		//Not working yet
	}
	
	UpdateView();
}

void EXP_Camera::TranslateCamera(vec3f trans, bool changeSub) {
	RD_Camera::TranslateCamera(trans, changeSub);
	EXP_Component::m_pos += trans;
}

void EXP_Camera::RotateCamera(vec3f rotation) {
	RD_Camera::RotateCamera(rotation);
	EXP_Component::m_rot += rotation;
}

void EXP_Camera::AddPitch(float pitch) {
	RD_Camera::AddPitch(pitch);
	EXP_Component::m_rot += vec3f(0.0f, pitch, 0.0f);
}

void EXP_Camera::AddYaw(float yaw) {
	RD_Camera::AddYaw(yaw);
	EXP_Component::m_rot += vec3f(yaw, 0.0f, 0.0f);
}

void EXP_Camera::AddRoll(float roll) {
	RD_Camera::AddRoll(roll);
	EXP_Component::m_rot += vec3f(0.0f, 0.0f, roll);
}

void EXP_Camera::SetYPR(vec3f YPR) {
	RD_Camera::SetYPR(YPR);
	EXP_Component::m_rot = YPR;
}