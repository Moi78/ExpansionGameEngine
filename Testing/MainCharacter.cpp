#include "MainCharacter.h"

MainCharacter::MainCharacter(EXP_Game* gameinstance, vec3f spawnpoint) : EXP_Actor(gameinstance, spawnpoint, vec3f(), vec3f(), CL_VDFUNCPTR(MainCharacter::Tick), CL_VDFUNCPTR(MainCharacter::OnLoad)) {
	m_gameinstance = gameinstance;

	m_camera = new EXP_Camera(gameinstance, spawnpoint + vec3f(0.0f, 0.0f, 2.75f), vec3f(), vec3f(), vec3f(0.0f, 0.0f, 0.0f), 60.0f);
	m_bound = new EXP_RB_Box(gameinstance, spawnpoint + vec3f(0.0f, 0.0f, 1.5f), vec3f(), vec3f(0.5f, 0.5f, 3.0f), 50.0f);

	m_camera->Use();

	m_forwd = new EXP_KeyboardCallback(gameinstance, CL_VDFUNCPTR(MainCharacter::MoveForward), GLFW_KEY_W);
}

MainCharacter::~MainCharacter() {
	delete m_camera;
	delete m_bound;
}

void MainCharacter::Tick() {
	m_camera->SetLocation(m_bound->GetWorldPosition());
}

void MainCharacter::OnLoad() {
	std::cout << "Load" << std::endl;
}

void MainCharacter::MoveForward() {
	std::cout << "Forward" << std::endl;
}