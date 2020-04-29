#include "pch.h"
#include "demoMap.h"

EXPGE_LEVEL_CODE(demoMap);

demoMap::demoMap(EXP_Game* game, EXP_MapLoader* mload) : EXP_Level(true, true) {
	m_game = game;

	cam = new EXP_Camera(game, vec3f(0.0f, -10.0f, 5.0f), vec3f(), vec3f(), vec3f(90.0f), 60.0f, 0.1f, 100.0f);
	cam->Use();
}

void demoMap::OnStart() {

}

void demoMap::OnTick() {
	if (glfwGetKey(m_game->GetRenderer()->GetGLFWwindow(), GLFW_KEY_H) == GLFW_PRESS) {
		m_game->UnloadCurrentMap();
	}
}