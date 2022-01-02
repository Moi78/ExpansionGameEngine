#include "pch.h"
#include "Character.h"

Character::Character(EXP_Game* game) : EXP_Actor(game, vec3f(0.0f, 0.0f, 0.0f), vec3f(), vec3f(1.0f, 1.0f, 1.0f)) {
	m_cam = new EXP_Camera(game, vec3f(0.0f, 0.0f, 0.125f), vec3f(), vec3f(), vec3f(), 60.0f, 0.001f, 1000.0f);
	LinkComponent(m_cam);

	m_move = new EXP_KeyboardCallback(game, CL_VDFUNCPTR(Character::MoveForward), GLFW_KEY_W);
	m_destroy = new EXP_KeyboardCallback(game, CL_VDFUNCPTR(Character::DestroyActor), GLFW_KEY_H, true);
	m_roll = new EXP_KeyboardCallback(game, CL_VDFUNCPTR(Character::Roll), GLFW_KEY_R, false);
	m_getpos = new EXP_KeyboardCallback(game, CL_VDFUNCPTR(Character::PrnPos), GLFW_KEY_T, true);
	m_keyup = new EXP_KeyboardCallback(game, CL_VDFUNCPTR(Character::StopMove), GLFW_KEY_W, false, true);

	m_bound = new EXP_CharControllerCapsule(game, vec3f(0.0f, 0.0f, 10.0f), vec3f(0.0f, 90.0f), 3.0f, 0.5f, 1.0f);
	
	m_test = new EXP_StaticMesh(game, game->GetShaderByFileRef("/shaders/mat_met_blue.exmtl"), "/cactus.msh", vec3f(1.0f, 1.0f), vec3f(), vec3f(0.05f, 0.05f, 0.05f));
	LinkComponent(m_test);
}

Character::~Character() {
	//m_game->UnregisterMesh(m_meshes);
	m_game->UnregisterKeyboardCallback(m_move);
	m_game->UnregisterKeyboardCallback(m_destroy);

	m_game->GetInputHandler()->CaptureCursor(false);
}

void Character::OnStart() {
	m_cam->Use();
	m_game->GetInputHandler()->CaptureCursor(true);
}

void Character::OnTick() {
	SetWorldPos(m_bound->GetWorldPosition());

	m_cam->AddPitch(m_game->GetInputHandler()->GetMouseYaxis() / -10);
	m_cam->AddYaw(m_game->GetInputHandler()->GetMouseXaxis() / -10);
}

void Character::MoveForward() {
	//AddWorldPos(m_cam->GetForwardVector() * 0.1f);
	m_bound->AddMovementInput((m_cam->GetForwardVector() * vec3f(1.0f, 1.0f, 0.0f)) * vec3f(1.0f, 1.0f, -9.18f) * 10.0f);
}

void Character::DestroyActor() {
	m_game->UnregisterActor(this);
}

void Character::Roll() {
	m_cam->AddRoll(0.1f);
}

void Character::PrnPos() {
	m_pos.DBGPrint();
}

void Character::StopMove() {
	m_bound->StopMovement();
}