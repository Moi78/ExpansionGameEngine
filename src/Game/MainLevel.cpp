#include "MainLevel.h"

EXPGE_LEVEL_CODE(MainLevel);

MainLevel::MainLevel(EXP_Game* game, EXP_MapLoader* mloader) : EXP_Level(true, true) {
	m_game = game;

	m_player = new Character(game);

	RD_MeshBuilder* mb = new RD_MeshBuilder(game->GetRenderer(), game->GetShaderByFileRef("/shaders/mat_suzanne.exmtl"), vec3f(0.0f, 0.0f, 22.0f), vec3f(), vec3f(1.0f, 1.0f, 1.0f));

	std::vector<vec3f> positions = {
		vec3f(0.0f, 1.0f, 0.0f),
		vec3f(1.0f, -1.0f, 0.0f),
		vec3f(-1.0f, -1.0f, 0.0f)
	};

	std::vector<vec3f> normals = {
		vec3f(0.0f, 0.0f, 1.0f),
		vec3f(0.0f, 0.0f, 1.0f),
		vec3f(0.0f, 0.0f, 1.0f)
	};

	std::vector<vec2f> UVs = {
		vec2f(0.0f, 0.0f),
		vec2f(0.0f, 0.0f),
		vec2f(0.0f, 0.0f)
	};

	std::vector<unsigned int> ind = {
		0, 1, 2
	};

	mb->LoadMeshBuffer(positions, normals, UVs, ind);

	game->RegisterMesh(mb->GetMesh());
}

MainLevel::~MainLevel() {

}

void MainLevel::OnStart() {
	std::cout << "Game started" << std::endl;
}

void MainLevel::OnTick() {

}