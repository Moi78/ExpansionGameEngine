#include "pch.h"
#include "demoActor.h"

demoActor::demoActor(EXP_Game* game, vec3f spawn) : EXP_Actor(game, spawn, vec3f(), vec3f(1.0f, 1.0f, 1.0f)) {
	m_cam = new EXP_Camera(m_game, spawn + vec3f(0.0f, 0.0f, 1.0f), vec3f(), vec3f(), vec3f(90.0f, 90.0f, 0.0f), 60.0f, 0.01f, 1000.0f);
	m_bound = new EXP_RB_Box(m_game, spawn, vec3f(), vec3f(1.0f, 1.0f, 2.0f), 20.0f);
	m_plight = new EXP_PointLight(game, spawn, vec3f(0.0f, 1.0f, 0.0f), 5.0f, 10.0f);

	m_moveforwd = new EXP_KeyboardCallback(m_game, CL_VDFUNCPTR(demoActor::MoveForward), GLFW_KEY_Z);
	m_kaboom = new EXP_KeyboardCallback(m_game, CL_VDFUNCPTR(demoActor::Kaboom), GLFW_KEY_Y, true);
}

demoActor::~demoActor() {

}

void demoActor::Start() {
	m_cam->Use();
	m_game->GetInputHandler()->CaptureCursor(true);
}

void demoActor::Tick() {
	m_cam->SetPosition(m_bound->GetWorldPosition() + vec3f(0.0f, 0.0f, 1.0f));
	m_plight->SetPosition(m_bound->GetWorldPosition());
	m_cam->AddPitch(m_game->GetInputHandler()->GetMouseYaxis() / -10);
	m_cam->AddYaw(m_game->GetInputHandler()->GetMouseXaxis() / -10);

	//m_game->UpdatePhysics();
	//m_bound->AddMovementInput(vec3f(), 0.0f);
}

void demoActor::Unregister() {

}

void demoActor::MoveForward() {
	m_bound->AddMovementInput(m_cam->GetForwardVector() * vec3f(1.0f, 1.0f, 0.0f), 5.0f);
}

void demoActor::Kaboom() {
	m_game->PlaySimpleSound("/explosion.wav", 1.0f);
}