#include "EXP_PointLightActor.h"

EXP_PointLightActor::EXP_PointLightActor(EXP_Game *game, vec3 pos, vec3 color, float brightness, float radius) :
        EXP_Actor(pos, vec3(), vec3(1.0, 1.0, 1.0)) {
    m_plight = std::make_shared<EXP_PointLight>(game, vec3(), color, brightness, radius);
    LinkComponent(m_plight);

    game->GetRenderer()->RegisterPointLight(m_plight);
}