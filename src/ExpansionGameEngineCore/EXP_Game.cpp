#include "EXP_Game.h"

#include "EXP_HotLoad.h"
#include "EXP_MapLoader.h"
#include "EXP_Level.h"
#include "EXP_Material.h"

EXP_Game::EXP_Game(std::shared_ptr<RaindropRenderer> rndr, EXP_GameInfo gameinfo) {
    m_rndr = rndr;
    m_gameinfo = std::move(gameinfo);

    m_hotloader = std::make_shared<EXP_HotLoad>();
    m_maploader = std::make_unique<EXP_MapLoader>(this, m_hotloader);

    m_materials = EXP_GenericRessourceManager<std::shared_ptr<EXP_Material>>();
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

        m_currentLevel->TickActors();
    }
}

void EXP_Game::LoadLevel(std::string levelPath) {
    m_currentLevel = m_maploader->LoadLevel(levelPath);
}

std::shared_ptr<RaindropRenderer> EXP_Game::GetRenderer() {
    return m_rndr;
}

std::string EXP_Game::GetGameContentPath() {
    return m_gameinfo.RootGameContentDir;
}

std::string EXP_Game::GetEngineContentPath() {
    return m_gameinfo.RootEngineContentDir;
}

std::shared_ptr<EXP_Material> EXP_Game::QueryMaterial(std::string matPath) {
    if(m_materials.DoIDExists(matPath)) {
        return m_materials.GetRessource(matPath);
    }

    auto material = std::make_shared<EXP_Material>(this);
    if(!material->LoadMaterial(matPath)) {
        std::cout << "ERROR: Failed to load material" << std::endl;
        return nullptr; // TODO: Create a default material to be returned
    }
    m_materials.AddRessource(material, matPath);

    return material;
}