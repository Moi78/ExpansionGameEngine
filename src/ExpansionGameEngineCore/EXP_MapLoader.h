#ifndef EXPGE_EXP_MAPLOADER_H
#define EXPGE_EXP_MAPLOADER_H

#include <memory>
#include <fstream>
#include <string>

#include "EXP_HotLoad.h"
#include "EXP_Level.h"
#include "EXP_StaticMeshActor.h"
#include "EXP_DirLightActor.h"
#include "EXP_PointLightActor.h"

#include <jsoncpp/json/json.h>

class EXP_MapLoader {
public:
    EXP_MapLoader(EXP_Game* game, std::shared_ptr<EXP_HotLoad> gamelib);
    ~EXP_MapLoader();

    std::shared_ptr<EXP_Level> LoadLevel(std::string levelPath);

    std::shared_ptr<EXP_StaticMeshActor> CreateMesh(Json::Value node);
    std::shared_ptr<EXP_DirLightActor> CreateDirLight(Json::Value node);
    std::shared_ptr<EXP_PointLightActor> CreatePointLight(Json::Value node);

private:
    EXP_Game* m_game;
    std::shared_ptr<EXP_HotLoad> m_gameLib;
};

TYPEDEF(std::shared_ptr<EXP_Level>, LEVELCODEHANDLER, EXP_Game*);

#endif //EXPGE_EXP_MAPLOADER_H
