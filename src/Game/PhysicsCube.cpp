#include "PhysicsCube.h"

PhysicsCube::PhysicsCube(EXP_Game* game, vec3 pos) : EXP_Actor(pos, vec3(), vec3(1.0f, 1.0f, 1.0f)) {
    m_collision = std::make_shared<EXP_BoxBody>(game->GetPhysicsHandler(), pos, vec3(1.0f, 1.0f, 1.0f), vec3(), false);

    m_cube = std::make_shared<EXP_StaticMesh>(game, "/mdl/cube.msh", game->QueryMaterial("/mat/mat_test.json"), vec3(), vec3(), vec3(1.0, 1.0, 1.0));
    LinkComponent(m_cube);
}

void PhysicsCube::OnStart() {

}

void PhysicsCube::OnTick() {
    SetActorPos(m_collision->GetBodyPos());

    //vec3 p = m_collision->GetBodyPos();
    //std::cout << p.GetX() << " " << p.GetY() << " " << p.GetZ() << std::endl;
}
