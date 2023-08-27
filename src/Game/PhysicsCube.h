#ifndef EXPGE_PHYSICSCUBE_H
#define EXPGE_PHYSICSCUBE_H

#include <EXP_Game.h>
#include <EXP_Actor.h>
#include <physics/EXP_BoxBody.h>
#include <EXP_StaticMesh.h>

class PhysicsCube : public EXP_Actor {
public:
    PhysicsCube(EXP_Game* game, vec3 pos, vec3 rot, vec3 scale);

    void OnStart() override;
    void OnTick() override;

private:
    std::shared_ptr<EXP_BoxBody> m_collision;
    std::shared_ptr<EXP_StaticMesh> m_cube;
};


#endif //EXPGE_PHYSICSCUBE_H
