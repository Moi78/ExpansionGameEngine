#include "MainLevel.h"

EXPGE_LEVEL_CODE(MainLevel);

MainLevel::MainLevel(EXP_Game* game, EXP_MapLoader* mloader) : EXP_Level(true, true) {
	m_game = game;
	m_mloader = mloader;

	m_player = new Character(game);

	m_ch_lvl = new EXP_KeyboardCallback(game, CL_VDFUNCPTR(MainLevel::ChangeLVL), GLFW_KEY_Y, true);

	RD_ShaderLoader* shd = game->GetRenderer()->GetRenderingAPI()->CreateShader();

	BD_MatCustomShaderRead mr("Content/shaders/mat_particle.exmtl");

	std::string frag = mr.GetShaderCode();
	std::string vert = getFileData("Engine/Shaders/glsl/Particles.vert");

	shd->CompileShaderFromCode(vert, frag);

	RD_ShaderMaterial* mat = new RD_ShaderMaterial(shd);
	for (int i = 0; i < mr.GetTextureCount(); i++) {
		RD_Texture* t = game->GetRenderer()->GetRenderingAPI()->CreateTexture();
		t->LoadTexture(mr.GetTexturePath(i));
		mat->AddTexture(mr.GetTextureParamName(i), t);
	}

	mr.CloseFile();

	RD_SmallParticleEmitter* part = new RD_SmallParticleEmitter(
		game->GetRenderer(), mat,
		vec3f(0.0f, 0.0f, 3.0f), vec3f(1.0f, 0.0f, 1.0f),
		50.0f, 0.1f, 5.0f
	);
	game->GetRenderer()->RegisterParticleEmitter(part);

	part->SetEmittingDirectionRandomness(0.5f);
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