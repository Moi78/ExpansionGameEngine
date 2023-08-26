#include "EXP_BoxBodyActor.h"

EXP_BoxBodyActor::EXP_BoxBodyActor(std::shared_ptr<EXP_PhysicsHandler> handler, vec3 pos, vec3 rot, vec3 scale) : EXP_Actor(pos, rot, scale) {
    m_body = std::make_unique<EXP_BoxBody>(handler, pos, scale, rot, true);
}

void EXP_BoxBodyActor::OnStart() {

}

void EXP_BoxBodyActor::OnTick() {

}