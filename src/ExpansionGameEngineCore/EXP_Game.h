#ifndef EXPGE_EXP_GAME_H
#define EXPGE_EXP_GAME_H

#include<memory>
#include <string>

#include <RaindropRenderer.h>

#include "EXP_Conf.h"

//Forward decl.
class EXP_HotLoad;
class EXP_MapLoader;
class EXP_Level;

class EXP_Game {
public:
    EXP_Game(std::shared_ptr<RaindropRenderer> rndr, EXP_GameInfo gameinfo);
    ~EXP_Game();

    //Engine control
    bool InitEngine();
    void RunGame();

    void LoadLevel(std::string levelPath);

private:
    std::shared_ptr<RaindropRenderer> m_rndr;

    std::shared_ptr<EXP_HotLoad> m_hotloader;
    std::unique_ptr<EXP_MapLoader> m_maploader;

    std::shared_ptr<EXP_Level> m_currentLevel;

    EXP_GameInfo m_gameinfo;
};


#endif //EXPGE_EXP_GAME_H
