#ifndef EXPGE_EXP_BOXBODY_H
#define EXPGE_EXP_BOXBODY_H

#include "EXP_PhysicsHandler.h"
#include "EXP_Component.h"
#include "EXP_Body.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>

class EXP_BoxBody : public EXP_Body {
public:
    EXP_BoxBody(std::shared_ptr<EXP_PhysicsHandler> handler, vec3 pos, vec3 scale, vec3 rot, bool isStatic);
    ~EXP_BoxBody() = default;
};


#endif //EXPGE_EXP_BOXBODY_H
