#include "MainCharacter.h"

MainCharacter::MainCharacter(EXP_Game* gameinstance, vec3f spawnpoint) {
	m_gameinstance = gameinstance;

	m_camera = new EXP_Camera(gameinstance, spawnpoint + vec3f(0.0f, 0.0f, 2.75f), vec3f(), vec3f(), vec3f(0.0f, 0.0f, 0.0f), 60.0f);
	m_bound = new EXP_RB_Box(gameinstance, spawnpoint + vec3f(0.0f, 0.0f, 1.5f), vec3f(), vec3f(0.5f, 0.5f, 3.0f), 50.0f);

	m_camera->Use();
}

MainCharacter::~MainCharacter() {
	delete m_camera;
	delete m_bound;
}

void MainCharacter::UpdateCharacter() {
	m_camera->SetLocation(m_bound->GetWorldPosition());
	
	if (glfwGetKey(m_gameinstance->GetRenderer()->GetGLFWwindow(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
		m_camera->AddYaw(-1.0f);
	}

	if (glfwGetKey(m_gameinstance->GetRenderer()->GetGLFWwindow(), GLFW_KEY_LEFT) == GLFW_PRESS) {
		m_camera->AddYaw(1.0f);
	}

	if (glfwGetKey(m_gameinstance->GetRenderer()->GetGLFWwindow(), GLFW_KEY_UP) == GLFW_PRESS) {
		m_camera->AddPitch(1.0f);
	}

	if (glfwGetKey(m_gameinstance->GetRenderer()->GetGLFWwindow(), GLFW_KEY_DOWN) == GLFW_PRESS) {
		m_camera->AddPitch(-1.0f);
	}
}