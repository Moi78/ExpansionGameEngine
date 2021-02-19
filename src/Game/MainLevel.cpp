#include "MainLevel.h"

EXPGE_LEVEL_CODE(MainLevel);

MainLevel::MainLevel(EXP_Game* game, EXP_MapLoader* mloader) : EXP_Level(true, true) {
	m_game = game;
	m_mloader = mloader;

	m_player = new Character(game);

	m_ch_lvl = new EXP_KeyboardCallback(game, CL_VDFUNCPTR(MainLevel::ChangeLVL), GLFW_KEY_Y, true);
	m_floor = new EXP_RB_Box(game, vec3f(), vec3f(), vec3f(50.0f, 50.0f, 0.5f), 0.0f);

	EXP_GUI_Text* txt = new EXP_GUI_Text(game, "/acetone.ttf", "Hello world", 120, vec2f(100, 100), vec2f(500, 500));
	EXP_GUI_ColorCache* cache = new EXP_GUI_ColorCache(game, vec3f(0.0f, 1.0f, 0.0f), 1.0f, 100, 100, 0, 0);
}

MainLevel::~MainLevel() {

}

void MainLevel::OnStart() {
	std::cout << "Game started" << std::endl;

	m_mloader->GetStaticMeshByName("skydome")->SetShadowCasting(false);
}

void MainLevel::OnTick() {

}

void MainLevel::ChangeLVL() {
	m_game->LoadMap("/map/MainLevel2.json");
}