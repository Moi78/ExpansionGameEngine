#include "pch.h"
#include "demoMap.h"

EXPGE_LEVEL_CODE(demoMap);

demoMap::demoMap(EXP_Game* game, EXP_MapLoader* mload) : EXP_Level(true, true) {
	m_game = game;
	m_load = mload;

	cam = new EXP_Camera(game, vec3f(0.0f, -10.0f, 5.0f), vec3f(), vec3f(), vec3f(90.0f), 60.0f, 0.1f, 100.0f);
	cam->Use();

	m_key = new EXP_KeyboardCallback(m_game, CL_VDFUNCPTR(demoMap::KeyTest), GLFW_KEY_H);
}

demoMap::~demoMap() {
	delete m_key;
}

void demoMap::OnStart() {
	
}

void demoMap::OnTick() {
	if (glfwGetKey(m_game->GetRenderer()->GetGLFWwindow(), GLFW_KEY_W) == GLFW_PRESS) {
		m_game->UnloadCurrentMap();
	}
}

void demoMap::KeyTest() {
	std::cout << "Hello world" << std::endl;
}