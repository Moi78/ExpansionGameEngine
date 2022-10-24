#ifndef EXPGE_EXP_LEVEL_H
#define EXPGE_EXP_LEVEL_H

#include <memory>

#include "EXP_Game.h"
#include "EXP_Actor.h"

class EXP_Level {
public:
    EXP_Level(EXP_Game* game);
    virtual ~EXP_Level();

    void RegisterActor(std::shared_ptr<EXP_Actor> actor);
    void UnregisterActor(uint64_t actor_uid);

    void TickActors();

private:
    std::vector<std::shared_ptr<EXP_Actor>> m_actors;
};


#endif //EXPGE_EXP_LEVEL_H
