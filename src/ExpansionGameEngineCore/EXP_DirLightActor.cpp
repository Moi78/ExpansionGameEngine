#include "EXP_DirLightActor.h"

EXP_DirLightActor::EXP_DirLightActor(EXP_Game* game, vec3 dir, vec3 color, float brightness) : EXP_Actor(vec3(), vec3(), vec3()) {
    m_dlight_comp = std::make_shared<EXP_DirLight>(dir, color, brightness);

    game->GetRenderer()->RegisterDirLight(m_dlight_comp);
}