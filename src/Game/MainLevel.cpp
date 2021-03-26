#include "MainLevel.h"

EXPGE_LEVEL_CODE(MainLevel);

MainLevel::MainLevel(EXP_Game* game, EXP_MapLoader* mloader) : EXP_Level(true, true) {
	m_game = game;
	m_mloader = mloader;

	m_player = new Character(game);

	m_ch_lvl = new EXP_KeyboardCallback(game, CL_VDFUNCPTR(MainLevel::ChangeLVL), GLFW_KEY_Y, true);

	//EXP_SMParticleEmitter* part = new EXP_SMParticleEmitter(
	//	game, game->GetShaderByFileRefParticle("/shaders/mat_particle.exmtl"),
	//	vec3f(0.0f, 0.0f, 3.0f), vec3f(), vec3f(1.0f, 1.0f, 1.0f), vec3f(1.0f, 0.0f, 1.0f),
	//	50.0f, 0.5f, 5.0f
	//);

	//part->SetEmittingDirectionRandomness(0.5f);
}

MainLevel::~MainLevel() {

}

void MainLevel::OnStart() {
	std::cout << "Game started" << std::endl;

	//m_mloader->GetDirLightByName("sun")->SetShadowCasting(false);

	m_mloader->GetStaticMeshByName("skydome")->SetShadowCasting(false);
	m_game->GetRenderer()->SetAmbientStrength(0.1f);
	m_game->GetRenderer()->SetAmbientColor(vec3f(0.0f, 0.2f, 0.8f));
}

void MainLevel::OnTick() {

}

void MainLevel::ChangeLVL() {
	m_game->LoadMap("/map/MainLevel2.json");
}