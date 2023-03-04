#ifndef EXPGE_EXP_LEVEL_H
#define EXPGE_EXP_LEVEL_H

#include <memory>
#include <unordered_map>
#include <utility>
#include <string>

#include "EXP_Game.h"
#include "EXP_Actor.h"

class EXP_Level {
public:
    EXP_Level(EXP_Game* game);
    virtual ~EXP_Level();

    void RegisterActor(std::shared_ptr<EXP_Actor> actor, std::optional<std::string> name = {});
    void UnregisterActor(uint64_t actor_uid);
    std::shared_ptr<EXP_Actor> FindActorByUID(uint64_t uid);

    template<class T>
    std::shared_ptr<T> GetCastedActorByName(std::string name) {
        if(m_name_id.find(name) == m_name_id.end()) {
            std::cerr << "ERROR: Actor does not exists." << std::endl;
            return {};
        }

        auto act = FindActorByUID(m_name_id[name]);
        return std::reinterpret_pointer_cast<T>(act);
    }

    void TickActors();

    virtual void OnStart() {};
    virtual void OnTick() {};

private:
    std::vector<std::shared_ptr<EXP_Actor>> m_actors;
    std::unordered_map<std::string, uint64_t> m_name_id;
};


#endif //EXPGE_EXP_LEVEL_H
