#include "EXP_Level.h"

EXP_Level::EXP_Level(EXP_Game *game) {

}

EXP_Level::~EXP_Level() {

}

void EXP_Level::RegisterActor(std::shared_ptr<EXP_Actor> actor, std::optional<std::string> name) {
    m_actors.push_back(actor);

    if(name.has_value()) {
        m_name_id.insert(std::pair<std::string, uint64_t>(name.value(), actor->GetActorUID()));
    }
}

void EXP_Level::UnregisterActor(uint64_t actor_uid) {
    for(int i = 0; i < m_actors.size(); i++) {
        if(m_actors[i]->GetActorUID() == actor_uid) {
            m_actors.erase(m_actors.begin() + i);
            break;
        }
    }
    return;
}

std::shared_ptr<EXP_Actor> EXP_Level::FindActorByUID(uint64_t uid) {
    for(auto& act : m_actors) {
        if(act->GetActorUID() == uid) {
            return act;
        }
    }

    std::cerr << "ERROR: Actor" << uid << " not found." << std::endl;
    return {};
}

void EXP_Level::TickActors() {
    for(auto& a : m_actors) {
        a->OnTick();
    }
}