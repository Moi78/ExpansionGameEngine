#include "pch.h"
#include "cave.h"

EXPGE_LEVEL_CODE(cave);

cave::cave(EXP_Game* game, EXP_MapLoader* mloader) : EXP_Level(true, true) {
	m_game = game;
	m_mloader = mloader;

	m_character = new Character(game);

	m_gui_cache = new EXP_GUI_ColorCache(game, vec3f(0.0, 0.5, 1.0), 0.5f, 100.0f, 100.0f, 0.0f, 0.0f);

	m_gui_tex = new EXP_GUI_ImageTexture(game, "/noice.png", 0.5f, 200.0f, 200.0f, 100.0f, 100.0f);

	m_meshes = new EXP_InstanciatedMesh(game, game->GetShaderByFileRef("/shaders/mat_room.exmtl"), "/meshes/sphere");

	for (int i = 0; i < 10; i++) {
		for (int y = 0; y < 10; y++) {
			m_meshes->AppendInstance({ vec3f(0.0f, (float)y * 2, (float)i * 2), vec3f(), vec3f(1.0f, 1.0f, 1.0f) });
		}
	}

	EXP_StaticMesh* test_axis = new EXP_StaticMesh(game, game->GetShaderByFileRef("/shaders/mat_room.exmtl"), "/meshes/test_axis", vec3f(-15.0f), vec3f(45.0f, 0.0f), vec3f(1.0f, 1.0f, 1.0f));
	//test_axis->SetShadowCasting(false);

	vec3f a(1.0f, 2.0f, 4.0f);
	vec4f vtest(a, 1.0f);

	mat4f t(1.0f);
	t = TranslateMatrix(t, vec3f(2.0f, 4.0f, 8.0f));
	t = ScaleMatrix(t, vec3f(1.0f, 4.0f, 5.0f));
	t = RotateMatrix(t, vec3f(50.0f, 20.0f, 80.0f));

	//t.DBG_print_matrix();
	//svtest.DBGPrint();
}

cave::~cave() {

}

void cave::OnStart() {
	std::cout << "Game started" << std::endl;
	//m_mloader->GetDirLightByName("sun")->SetLightColor(vec3f(1.0f, 0.0f, 0.0f));

	m_game->GetRenderer()->SetAmbientStrength(0.2f);
}

void cave::OnTick() {

}

void cave::Click() {
	m_gui_tex->SetPosition(vec3f(500, 300, 0));
}