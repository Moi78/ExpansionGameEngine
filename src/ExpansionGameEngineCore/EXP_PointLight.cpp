#include "EXP_PointLight.h"

EXP_PointLight::EXP_PointLight(EXP_Game* game, vec3 pos, vec3 color, float brightness, float radius) :
EXP_Component(pos, vec3(), vec3(1.0, 1.0, 1.0)), RD_PointLight(pos, brightness, color, radius)
{
    m_game = game;
}

void EXP_PointLight::SetParentMatrix(mat4f &matrix) {
    vec4 pos = vec4(EXP_Component::m_pos, 1.0f);
    vec4 rpos = matrix * pos;

    RD_PointLight::m_pos = rpos.ToVec3();
    m_game->GetRenderer()->UpdateLighting();
}