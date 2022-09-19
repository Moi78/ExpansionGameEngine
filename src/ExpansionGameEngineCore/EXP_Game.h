#ifndef EXPGE_EXP_GAME_H
#define EXPGE_EXP_GAME_H

#include <memory>
#include <string>

#include <RaindropRenderer.h>

#include "EXP_Conf.h"

class EXP_Game {
public:
    EXP_Game(std::shared_ptr<RaindropRenderer> rndr, EXP_GameInfo gameinfo);
    ~EXP_Game();

    //Engine control
    bool InitEngine();
    void RunGame();

private:
    std::shared_ptr<RaindropRenderer> m_rndr;

    EXP_GameInfo m_gameinfo;
};


#endif //EXPGE_EXP_GAME_H
