#ifndef EXPGE_EXP_POINTLIGHTACTOR_H
#define EXPGE_EXP_POINTLIGHTACTOR_H

#include <memory>

#include "EXP_Game.h"
#include "EXP_Actor.h"
#include "EXP_PointLight.h"

class EXP_PointLightActor : public EXP_Actor {
public:
    EXP_PointLightActor(EXP_Game* game, vec3 pos, vec3 color, float brightness, float radius);
    ~EXP_PointLightActor() {};

    void OnStart() override {}
    void OnTick() override {}

private:
    std::shared_ptr<EXP_PointLight> m_plight;
};


#endif //EXPGE_EXP_POINTLIGHTACTOR_H
