#ifndef EXPGE_EXP_SKELETALMESHACTOR_H
#define EXPGE_EXP_SKELETALMESHACTOR_H

#include <memory>

#include "EXP_Game.h"
#include "EXP_Actor.h"
#include "EXP_SkeletalMesh.h"

class EXP_SkeletalMeshActor : public EXP_Actor {
public:
    EXP_SkeletalMeshActor(EXP_Game* game, std::string mesh_file, std::string skel_file, std::shared_ptr<EXP_Material> mat, vec3 pos, vec3 rot, vec3 scale);
    ~EXP_SkeletalMeshActor() {};

    void OnStart() override {}
    void OnTick() override {}

private:
    std::shared_ptr<EXP_SkeletalMesh> m_skel_mesh;
};


#endif //EXPGE_EXP_SKELETALMESHACTOR_H
