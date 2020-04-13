#include "MainCharacter.h"

MainCharacter::MainCharacter(EXP_Game* gameinstance, vec3f spawnpoint) : EXP_Actor(gameinstance, spawnpoint, vec3f(), vec3f(), CL_VDFUNCPTR(MainCharacter::Tick), CL_VDFUNCPTR(MainCharacter::OnLoad)) {
	m_gameinstance = gameinstance;

	m_camera = new EXP_Camera(gameinstance, spawnpoint + vec3f(0.0f, 0.0f, 2.75f), vec3f(), vec3f(), vec3f(0.0f, 0.0f, 0.0f), 60.0f);
	m_bound = new EXP_RB_Box(gameinstance, spawnpoint + vec3f(0.0f, 0.0f, 1.5f), vec3f(), vec3f(0.5f, 0.5f, 3.0f), 50.0f);

	m_camera->Use();

	m_forwd = new EXP_KeyboardCallback(gameinstance, CL_VDFUNCPTR(MainCharacter::MoveForward), GLFW_KEY_W);
	m_back = new EXP_KeyboardCallback(gameinstance, CL_VDFUNCPTR(MainCharacter::MoveBack), GLFW_KEY_S);

	m_camYM = new EXP_KeyboardCallback(gameinstance, CL_VDFUNCPTR(MainCharacter::CamMYaw), GLFW_KEY_RIGHT);
	m_camYP = new EXP_KeyboardCallback(gameinstance, CL_VDFUNCPTR(MainCharacter::CamPYaw), GLFW_KEY_LEFT);
	m_camPM = new EXP_KeyboardCallback(gameinstance, CL_VDFUNCPTR(MainCharacter::CamMPitch), GLFW_KEY_DOWN);
	m_camPP = new EXP_KeyboardCallback(gameinstance, CL_VDFUNCPTR(MainCharacter::CamPPitch), GLFW_KEY_UP);
}

MainCharacter::~MainCharacter() {
	delete m_bound;
}

void MainCharacter::Tick() {
	m_camera->SetLocation(m_bound->GetWorldPosition());
}

void MainCharacter::OnLoad() {
	
}

void MainCharacter::MoveForward() {
	m_bound->AddMovementInput(m_camera->GetSubject(), 3.0f);
}

void MainCharacter::MoveBack() {
	m_bound->AddMovementInput(m_camera->GetSubject(), -3.0f);
}

void MainCharacter::CamMYaw() {
	m_camera->AddYaw(-1.0f);
}

void MainCharacter::CamPYaw() {
	m_camera->AddYaw(1.0f);
}

void MainCharacter::CamMPitch() {
	m_camera->AddPitch(-1.0f);
}

void MainCharacter::CamPPitch() {
	m_camera->AddPitch(1.0f);
}