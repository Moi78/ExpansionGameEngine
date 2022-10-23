#include "EXP_Game.h"

#include "EXP_HotLoad.h"
#include "EXP_MapLoader.h"
#include "EXP_Level.h"

EXP_Game::EXP_Game(std::shared_ptr<RaindropRenderer> rndr, EXP_GameInfo gameinfo) {
    m_rndr = rndr;
    m_gameinfo = std::move(gameinfo);

    m_hotloader = std::make_shared<EXP_HotLoad>();
    m_maploader = std::make_unique<EXP_MapLoader>(this, m_hotloader);
}

EXP_Game::~EXP_Game() {

}

bool EXP_Game::InitEngine() {
    if(!m_rndr->InitRenderer(m_gameinfo.RootEngineContentDir)) {
        return false;
    }

    if(!m_hotloader->LoadLib(m_gameinfo.GameLib.c_str())) {
        return false;
    }

    LoadLevel(m_gameinfo.RootGameContentDir + m_gameinfo.StartLevel);

    return true;
}

void EXP_Game::RunGame() {
    while(!m_rndr->WantToClose()) {
        m_rndr->UpdateWindow();
        m_rndr->RenderScene();
    }
}

void EXP_Game::LoadLevel(std::string levelPath) {
    m_currentLevel = m_maploader->LoadLevel(levelPath);
    m_currentLevel->Test();
}