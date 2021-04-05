#include "MainLevel.h"

EXPGE_LEVEL_CODE(MainLevel);

MainLevel::MainLevel(EXP_Game* game, EXP_MapLoader* mloader) : EXP_Level(true, true) {
	m_game = game;
	m_mloader = mloader;

	m_player = new Character(game);

	m_ch_lvl = new EXP_KeyboardCallback(game, CL_VDFUNCPTR(MainLevel::ChangeLVL), GLFW_KEY_Y, true);

	EXP_InstanciatedMesh* inst = new EXP_InstanciatedMesh(game, game->GetShaderByFileRefInstanced("/shaders/mat_inst.exmtl"), "/rock1");

	for (int i = 0; i < 10; i++) {
		RD_MeshInstance in{};
		in.pos = vec3f(0.0f, 0.0f, i);
		in.rotation = vec3f();
		in.scale = vec3f(0.5f, 0.5f, 0.5f);

		inst->AppendInstance(in);
	}

	EXP_SMParticleEmitter* p = new EXP_SMParticleEmitter(
		game, game->GetShaderByFileRefInstanced("/shaders/mat_inst.exmtl"),
		vec3f(0.0f, 0.0f, 5.0f), vec3f(), vec3f(1.0f, 1.0f, 1.0f),
		vec3f(1.0f), 10.0f, 0.5f, 5.0f
	);
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