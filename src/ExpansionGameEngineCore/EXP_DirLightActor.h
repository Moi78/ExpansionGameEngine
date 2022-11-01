#ifndef EXPGE_EXP_DIRLIGHTACTOR_H
#define EXPGE_EXP_DIRLIGHTACTOR_H

#include "EXP_Game.h"
#include "EXP_Actor.h"
#include "EXP_DirLight.h"

class EXP_DirLightActor : public EXP_Actor {
public:
    EXP_DirLightActor(EXP_Game* game, vec3 dir, vec3 color, float brightness);
    ~EXP_DirLightActor() {};

    void OnStart() override {}
    void OnTick() override {}

private:
    std::shared_ptr<EXP_DirLight> m_dlight_comp;
};


#endif //EXPGE_EXP_DIRLIGHTACTOR_H
