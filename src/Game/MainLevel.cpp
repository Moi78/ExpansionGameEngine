#include "MainLevel.h"

EXPGE_LEVEL_CODE(MainLevel);

MainLevel::MainLevel(EXP_Game* game, EXP_MapLoader* mloader) : EXP_Level(true, true) {
	m_game = game;
	m_mloader = mloader;

	m_player = new Character(game);

	m_ch_lvl = new EXP_KeyboardCallback(game, CL_VDFUNCPTR(MainLevel::ChangeLVL), GLFW_KEY_Y, true);
	m_floor = new EXP_RB_Box(game, vec3f(), vec3f(), vec3f(50.0f, 50.0f, 0.5f), 0.0f);

	m_game->GetRenderer()->DisableFeature(RendererFeature::Bloom);

	RD_ShaderLoader* ld = game->GetRenderer()->GetRenderingAPI()->CreateShader();
	ld->compileShaderFromFile(
		game->GetGameInfo().RootEngineContentFolder + "/Shaders/glsl/TextRender.vert",
		game->GetGameInfo().RootEngineContentFolder + "/Shaders/glsl/TextRender.frag"
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