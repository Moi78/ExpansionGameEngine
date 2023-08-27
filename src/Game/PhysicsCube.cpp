#include "PhysicsCube.h"

PhysicsCube::PhysicsCube(EXP_Game* game, vec3 pos, vec3 rot, vec3 scale) : EXP_Actor(pos, rot, scale) {
    m_collision = std::make_shared<EXP_BoxBody>(game->GetPhysicsHandler(), pos, scale, rot, false);

    m_cube = std::make_shared<EXP_StaticMesh>(game, "/mdl/cube.msh", game->QueryMaterial("/mat/mat_test.json"), vec3(), vec3(), vec3(1.0f, 1.0f, 1.0f));
    LinkComponent(m_cube);
}

void PhysicsCube::OnStart() {

}

void PhysicsCube::OnTick() {
    //SetActorPos(m_collision->GetBodyPos());
    m_transform = m_collision->GetBodyTransform();
    //m_transform.DBG_print_matrix();
    EXP_Actor::UpdateActor();

    //vec3 p = m_collision->GetBodyPos();
    //std::cout << p.GetX() << " " << p.GetY() << " " << p.GetZ() << std::endl;
}
