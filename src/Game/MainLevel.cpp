#include "MainLevel.h"

EXPGE_LEVEL_CODE(MainLevel);

MainLevel::MainLevel(EXP_Game* game, EXP_MapLoader* mloader) : EXP_Level(true, true) {
	m_game = game;

	m_player = new Character(game);

	m_ch_lvl = new EXP_KeyboardCallback(game, CL_VDFUNCPTR(MainLevel::ChangeLVL), GLFW_KEY_Y, true);
}

MainLevel::~MainLevel() {

}

void MainLevel::OnStart() {
	std::cout << "Game started" << std::endl;
}

void MainLevel::OnTick() {

}

void MainLevel::ChangeLVL() {
	m_game->LoadMap("/map/MainLevel2.json");
}