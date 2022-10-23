#include "EXP_Level.h"

EXP_Level::EXP_Level(EXP_Game *game) {

}

EXP_Level::~EXP_Level() {

}

void EXP_Level::RegisterActor(std::shared_ptr<EXP_Actor> actor) {
    m_actors.push_back(actor);
}

void EXP_Level::UnregisterActor(uint64_t actor_uid) {
    return;
}