#include "pch.h"
#include "Character.h"

Character::Character(EXP_Game* game) : EXP_Actor(game, vec3f(0.0f, 0.0f, 0.0f), vec3f(), vec3f(1.0f, 1.0f, 1.0f)) {
	m_cam = new EXP_Camera(game, vec3f(1.0f, 0.0f, 1.0f), vec3f(), vec3f(), vec3f(), 60.0f, 0.001f, 1000.0f);
	LinkComponent(m_cam);

	m_move = new EXP_KeyboardCallback(game, CL_VDFUNCPTR(Character::MoveForward), GLFW_KEY_W);
	m_destroy = new EXP_KeyboardCallback(game, CL_VDFUNCPTR(Character::DestroyActor), GLFW_KEY_H, true);
}

Character::~Character() {
	//m_game->UnregisterMesh(m_meshes);
	m_game->UnregisterKeyboardCallback(m_move);
	m_game->UnregisterKeyboardCallback(m_destroy);

	m_game->GetInputHandler()->CaptureCursor(false);
}

void Character::Start() {
	m_cam->Use();
	m_game->GetInputHandler()->CaptureCursor(true);
}

void Character::Tick() {
	m_cam->AddPitch(m_game->GetInputHandler()->GetMouseYaxis() / -10);
	m_cam->AddYaw(m_game->GetInputHandler()->GetMouseXaxis() / -10);
}

void Character::MoveForward() {
	AddWorldPos(m_cam->GetForwardVector() * 0.1);
}

void Character::DestroyActor() {
	m_game->UnregisterActor(this);
}