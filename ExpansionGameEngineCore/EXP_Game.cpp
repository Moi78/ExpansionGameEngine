#include "pch.h"
#include "EXP_Game.h"

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

void EXP_Game::LaunchMainLoop() {
	while (!m_rndr->WantToClose()) {
		m_rndr->ClearWindow(m_refreshColor);

		m_rndr->SwapWindow();
	}
}

BD_GameInfo EXP_Game::GetGameInfo() {
	return m_gameinfo;
}

RaindropRenderer* EXP_Game::GetRenderer() {
	return m_rndr;
}