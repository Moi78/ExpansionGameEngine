#ifndef EXPGE_EXP_GENERICRESSOURCEMANAGER_H
#define EXPGE_EXP_GENERICRESSOURCEMANAGER_H

#include <unordered_map>
#include <string>

template<typename T>
class EXP_GenericRessourceManager {
public:
    EXP_GenericRessourceManager() {};
    ~EXP_GenericRessourceManager() {};

    void AddRessource(T res, std::string id) {
        m_ressources.insert({id, res});
    }

    T GetRessource(std::string id) {
        return m_ressources[id];
    }

    void RemoveRessource(std::string id) {
        m_ressources.erase(id);
    }
    void PurgeRessources() {
        m_ressources.empty();
    }

    bool DoIDExists(std::string id) {
        return m_ressources.find(id) != m_ressources.end();
    }

protected:
    std::unordered_map<std::string, T> m_ressources;
};


#endif //EXPGE_EXP_GENERICRESSOURCEMANAGER_H
