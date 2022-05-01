#include "MainLevel.h"

EXPGE_LEVEL_CODE(MainLevel);

MainLevel::MainLevel(EXP_Game* game, EXP_MapLoader* mloader) : EXP_Level(true, true) {
	m_game = game;
	m_mloader = mloader;

	m_cap = new RD_ReflectionCapture(m_game->GetRenderer(), vec3f(0.0f, 0.0f, 3.0f));
	EXP_StaticMesh* vis = new EXP_StaticMesh(game, game->GetShaderByFileRef("/shaders/mat_red.exmtl"), "/sphere.msh", vec3f(0.0f, 0.0f, 3.0f), vec3f(), vec3f(0.25f, 0.25f, 0.25f));

	m_player = new Character(game);

	m_ch_lvl = new EXP_KeyboardCallback(game, CL_VDFUNCPTR(MainLevel::ChangeLVL), GLFW_KEY_Y, true);

	m_test = new EXP_RB_Sphere(game, vec3f(0.0f, 0.0f, 20.0f), vec3f(), 0.5f, 1.0f);
	//m_floor = new EXP_RB_Box(game, vec3f(), vec3f(), vec3f(100.0f, 100.0f, 0.1f), 0.0f, false);
	m_floor = new EXP_RB_Box(game, vec3f(), vec3f(), vec3f(10.0f, 10.0f, 1.0f), 0.0f);

	//m_txt = new EXP_TextSurface(game, "Hello", 512, "/nova.ttf", vec3f(0.0f, 0.0f, 2.0f), vec3f(), vec3f(0.2f, 0.2f, 0.2f), vec3f(1.0f, 0.5f, 0.5f));

	std::string game_pref = m_game->GetGameInfo().RootGameContentFolder;
	m_game->GetRenderer()->MakeEnvCubemapFromTexs(
		{
			game_pref + "/Cubemap/px.png",
			game_pref + "/Cubemap/nx.png",
			game_pref + "/Cubemap/py.png",
			game_pref + "/Cubemap/ny.png",
			game_pref + "/Cubemap/pz.png",
			game_pref + "/Cubemap/nz.png"
		}
	);

	m_game->ToggleDebugRendering(true);

	m_count = 0.0f;
}

MainLevel::~MainLevel() {
	delete m_cap;
}

void MainLevel::OnStart() {
	std::cout << "Game started" << std::endl;
	m_cap->RenderCapture();

	//m_mloader->GetDirLightByName("sun")->SetShadowCasting(false);

	m_mloader->GetStaticMeshByName("skydome")->SetShadowCasting(false);
	//m_game->GetRenderer()->SetAmbientStrength(0.1f);
	//m_game->GetRenderer()->SetAmbientColor(vec3f(0.0f, 0.2f, 0.8f));

	//m_game->GetRenderer()->SetVSync(false);

	//m_game->GetRenderer()->DisableFeature(RendererFeature::SSAO);
}

void MainLevel::OnTick() {
	//m_mloader->GetStaticMeshByName("rock1_1")->SetPosition(m_test->GetWorldPosition());

	m_count += 0.001f;
}

void MainLevel::ChangeLVL() {
	m_game->LoadMap("/map/MainLevel2.json");
}
