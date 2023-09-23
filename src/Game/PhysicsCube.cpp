#include "PhysicsCube.h"

PhysicsCube::PhysicsCube(EXP_Game* game, vec3 pos, vec3 rot, vec3 scale) : EXP_Actor(pos, rot, scale) {
    m_collision = std::make_shared<EXP_BoxBody>(game->GetPhysicsHandler(), pos, scale, rot, false);
    SetRootComponent(m_collision);

    m_cube = std::make_shared<EXP_StaticMesh>(game, "/mdl/cube.msh", game->QueryMaterial("/mat/mat_test.json"), vec3(), vec3(), vec3(1.0f, 1.0f, 1.0f));
    LinkComponent(m_cube);
}

void PhysicsCube::OnStart() {

}

void PhysicsCube::OnTick() {
    EXP_Actor::UpdateActor();
}
