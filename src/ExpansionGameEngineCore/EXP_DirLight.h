#ifndef EXPGE_EXP_DIRLIGHT_H
#define EXPGE_EXP_DIRLIGHT_H

#include <vec.h>

#include <RD_DirLight.h>

#include "EXP_Game.h"
#include "EXP_Component.h"

class EXP_DirLight : public EXP_Component, public RD_DirLight {
public:
    EXP_DirLight(vec3 dir, vec3 color, float brightness);
    ~EXP_DirLight() {};
};


#endif //EXPGE_EXP_DIRLIGHT_H
