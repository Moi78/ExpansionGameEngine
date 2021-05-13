#include "pch.h"
#include "CameraActor.h"

CameraActor::CameraActor(EXP_Game* game) : EXP_Actor(game, vec3f(0.0f, 0.0f, 0.0f), vec3f(), vec3f(1.0f, 1.0f, 1.0f)) {
	m_cam = new EXP_Camera(game, vec3f(-2.0f, 0.0f, 0.0f), vec3f(), vec3f(), vec3f(), 60.0f, 0.001f, 1000.0f);
	LinkComponent(m_cam);
}

CameraActor::~CameraActor() {
	m_game->GetInputHandler()->CaptureCursor(false);
}

void CameraActor::Start() {
	m_cam->Use();
}

void CameraActor::Tick() {

}