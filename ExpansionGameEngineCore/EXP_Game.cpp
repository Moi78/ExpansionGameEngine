#include "pch.h"
#include "EXP_Game.h"

#include "EXP_Actor.h"

EXP_Game::EXP_Game(BD_Resolution res, BD_GameInfo gameinfo, vec3f refreshColor, std::string gameName) {
	InitGame(res, refreshColor, gameName, gameinfo);
}

EXP_Game::~EXP_Game() {
	delete m_rndr;
}

void EXP_Game::InitGame(BD_Resolution res, vec3f refreshColor, std::string gameName, BD_GameInfo gameinfo) {
	m_res = res;
	m_refreshColor = refreshColor;
	m_gameName = gameName;
	m_gameinfo = gameinfo;

	m_rndr = new RaindropRenderer(res.x, res.y, gameName);
}

void EXP_Game::MainLoop() {
	m_rndr->ClearWindow(m_refreshColor);

	for (auto mesh : m_staticMeshes) {
		mesh->render();
	}

	if (RENDER_DBG) {
		m_rndr->RenderDbg();
	}

	m_rndr->SwapWindow();
}

BD_GameInfo EXP_Game::GetGameInfo() {
	return m_gameinfo;
}

RaindropRenderer* EXP_Game::GetRenderer() {
	return m_rndr;
}

void EXP_Game::RegisterMesh(RD_Mesh* mesh) {
	m_staticMeshes.push_back(mesh);
}

void EXP_Game::RegisterPointLight(RD_PointLight* ptlight) {
	m_points_light.push_back(ptlight);
	m_rndr->AppendLight(ptlight);
}

vec3f EXP_Game::GetRefreshColor() {
	return m_refreshColor;
}

std::vector<RD_Mesh*> EXP_Game::GetStaticMeshes() {
	return m_staticMeshes;
}