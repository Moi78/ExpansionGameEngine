#include "MainLevel.h"

EXPGE_LEVEL_CODE(MainLevel);

MainLevel::MainLevel(EXP_Game* game, EXP_MapLoader* mloader) : EXP_Level(true, true) {
	m_game = game;
	m_mloader = mloader;

	m_player = new Character(game);

	m_ch_lvl = new EXP_KeyboardCallback(game, CL_VDFUNCPTR(MainLevel::ChangeLVL), GLFW_KEY_Y, true);

	m_test = new EXP_RB_Sphere(game, vec3f(0.0f, 0.0f, 20.0f), vec3f(), 0.5f, 1.0f);
	//m_floor = new EXP_RB_Box(game, vec3f(), vec3f(), vec3f(100.0f, 100.0f, 0.1f), 0.0f, false);
	m_floor = new EXP_RB_Box(game, vec3f(), vec3f(), vec3f(10.0f, 10.0f, 1.0f), 0.0f);

	m_txt = new EXP_TextSurface(game, "Hello", 512, "/nova.ttf", vec3f(0.0f, 0.0f, 2.0f), vec3f(), vec3f(0.2f, 0.2f, 0.2f), vec3f(1.0f, 0.5f, 0.5f));

	//EXP_SMParticleEmitter* pe = new EXP_SMParticleEmitter(
	//	game, game->GetShaderByFileRefInstanced("/shaders/mat_met_blue.exmtl"),
	//	vec3f(0.0f, 0.0f, 1.0f), vec3f(), vec3f(1.f, 1.f, 1.f),
	//	vec3f(0.0f, 0.5f, 1.0f), 5.0f, 0.02f, 3.0f
	//);

	//RD_ShaderLoader* sl_vert = m_game->GetRenderer()->GetRenderingAPI()->CreateShader();
	//sl_vert->compileShaderFromFile("Content/cust_shad/Gshad_hmap.vert", "Content/cust_shad/mat.frag");
	//
	//RD_ShaderMaterial* mat = new RD_ShaderMaterial(sl_vert, m_game->GetRenderer());

	//RD_Texture* tex = m_game->GetRenderer()->GetRenderingAPI()->CreateTexture();
	//tex->LoadTexture("Content/heightmap.png");
	//mat->AddTexture("tex", tex);

	//mat->MakeSSBO();

	//m_game->GetRenderer()->GetMaterialLibrary()->AddMaterialToLib(mat, "custom_mat_test");
	//
	//EXP_StaticMesh* smesh = new EXP_StaticMesh(game, mat, "/plane_sub.msh", vec3f(0.0f, 0.0f, 7.0f), vec3f(), vec3f(1.0f, 1.0f, 1.0f));

	m_game->ToggleDebugRendering(true);
}

MainLevel::~MainLevel() {

}

void MainLevel::OnStart() {
	std::cout << "Game started" << std::endl;

	//m_mloader->GetDirLightByName("sun")->SetShadowCasting(false);

	m_mloader->GetStaticMeshByName("skydome")->SetShadowCasting(false);
	//m_game->GetRenderer()->SetAmbientStrength(0.1f);
	//m_game->GetRenderer()->SetAmbientColor(vec3f(0.0f, 0.2f, 0.8f));

	//m_game->GetRenderer()->SetVSync(false);
}

void MainLevel::OnTick() {
	m_mloader->GetStaticMeshByName("rock1_1")->SetPosition(m_test->GetWorldPosition());
}

void MainLevel::ChangeLVL() {
	m_game->LoadMap("/map/MainLevel2.json");
}
