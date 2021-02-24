#include "MainLevel.h"

EXPGE_LEVEL_CODE(MainLevel);

MainLevel::MainLevel(EXP_Game* game, EXP_MapLoader* mloader) : EXP_Level(true, true) {
	m_game = game;
	m_mloader = mloader;

	m_player = new Character(game);

	m_ch_lvl = new EXP_KeyboardCallback(game, CL_VDFUNCPTR(MainLevel::ChangeLVL), GLFW_KEY_Y, true);
	m_floor = new EXP_RB_Box(game, vec3f(), vec3f(), vec3f(50.0f, 50.0f, 0.5f), 0.0f);

	m_game->GetRenderer()->DisableFeature(RendererFeature::Bloom);

	EXP_TextSurface* txt = new EXP_TextSurface(
		game, "Hello world - Hey", 512, "/nova.ttf",
		vec3f(0.0f, 0.0f, 1.0f), vec3f(0.0f), vec3f(0.1f, 0.1f, 1.0f),
		vec3f(1.0f, 1.0f)
	);
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