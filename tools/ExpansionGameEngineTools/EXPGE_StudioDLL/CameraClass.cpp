#include "pch.h"
#include "CameraClass.h"

CameraClass::CameraClass(EXP_Game * game) : EXP_Actor(game, vec3f(), vec3f(), vec3f(1.0f, 1.0f, 1.0)) {
	m_mvt_allowed = false;

	m_cam = new EXP_Camera(game, vec3f(), vec3f(), vec3f(), vec3f(), 60.0f, 0.01f, 1000.0f);
	LinkComponent(m_cam);

	m_mb_cbk = new EXP_MouseButtonCallback(game, CL_VDFUNCPTR(CameraClass::AllowMoving), 1, true);
	m_forward = new EXP_KeyboardCallback(game, CL_VDFUNCPTR(CameraClass::Forward), 87);
	m_backward = new EXP_KeyboardCallback(game, CL_VDFUNCPTR(CameraClass::Backward), 83);
}

CameraClass::~CameraClass() {

}

void CameraClass::OnStart() {
	m_cam->Use();
}

void CameraClass::OnTick() {
	//m_game->GetInputHandler()->CaptureCursor(false);
	if (!m_game->GetInputHandler()->GetMouseButton(1)) {
		m_game->GetInputHandler()->CaptureCursor(false);
		m_mvt_allowed = false;
	}
	else {
		m_cam->AddPitch(m_game->GetInputHandler()->GetMouseYaxis() / -20);
		m_cam->AddYaw(m_game->GetInputHandler()->GetMouseXaxis() / -20);
	}
}

void CameraClass::AllowMoving() {
	m_game->GetRenderer()->GetRenderingAPI()->GetWindowingSystem()->SetCursorPos(0, 0);
	m_game->GetInputHandler()->CaptureCursor(true);
	m_mvt_allowed = true;
}

void CameraClass::Forward() {
	if (!m_mvt_allowed) {
		return;
	}

	AddWorldPos(m_cam->GetForwardVector() * 0.1f);
}

void CameraClass::Backward() {
	if (!m_mvt_allowed) {
		return;
	}

	AddWorldPos(m_cam->GetForwardVector() * -0.1f);
}