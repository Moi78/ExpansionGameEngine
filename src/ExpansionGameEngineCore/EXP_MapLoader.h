#ifndef EXPGE_EXP_MAPLOADER_H
#define EXPGE_EXP_MAPLOADER_H

#include <memory>
#include <fstream>
#include <string>

#include "EXP_HotLoad.h"
#include "EXP_Level.h"

#include <json/json.h>

class EXP_MapLoader {
public:
    EXP_MapLoader(EXP_Game* game, std::shared_ptr<EXP_HotLoad> gamelib);
    ~EXP_MapLoader();

    std::shared_ptr<EXP_Level> LoadLevel(std::string levelPath);

private:
    EXP_Game* m_game;
    std::shared_ptr<EXP_HotLoad> m_gameLib;
};

TYPEDEF(std::shared_ptr<EXP_Level>, LEVELCODEHANDLER, EXP_Game*);

#endif //EXPGE_EXP_MAPLOADER_H
