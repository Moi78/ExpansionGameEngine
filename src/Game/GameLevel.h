#ifndef EXPGE_GAMELEVEL_H
#define EXPGE_GAMELEVEL_H

#include <EXP_HotLoad.h>
#include <EXP_Game.h>
#include <EXP_MapLoader.h>
#include <EXP_Level.h>

class GameLevel : public EXP_Level {
public:
    GameLevel(EXP_Game* game);
    ~GameLevel();
};

EXPGE_LEVEL_HEADER(GameLevel);

#endif //EXPGE_GAMELEVEL_H
