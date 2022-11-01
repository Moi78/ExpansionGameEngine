#ifndef EXPGE_EXP_POINTLIGHT_H
#define EXPGE_EXP_POINTLIGHT_H

#include <vec.h>
#include <mat4.h>

#include <RD_PointLight.h>

#include "EXP_Game.h"
#include "EXP_Component.h"

class EXP_PointLight : public EXP_Component, public RD_PointLight {
public:
    EXP_PointLight(EXP_Game* game, vec3 pos, vec3 color, float brightness, float radius);
    ~EXP_PointLight() {}

    void SetParentMatrix(mat4f& matrix) override;

private:
    EXP_Game* m_game;
};


#endif //EXPGE_EXP_POINTLIGHT_H
