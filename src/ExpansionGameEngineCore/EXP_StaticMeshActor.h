#ifndef EXPGE_EXP_STATICMESHACTOR_H
#define EXPGE_EXP_STATICMESHACTOR_H

#include "EXP_Actor.h"
#include "EXP_StaticMesh.h"

class EXP_StaticMeshActor : public EXP_Actor {
public:
    EXP_StaticMeshActor(EXP_Game* game, std::string mesh_file, std::shared_ptr<EXP_Material> mat, vec3 pos, vec3 rot, vec3 scale);
    virtual ~EXP_StaticMeshActor() {};

    void OnStart() override {}
    void OnTick() override {}

private:
    std::shared_ptr<EXP_StaticMesh> m_smesh_comp;
};


#endif //EXPGE_EXP_STATICMESHACTOR_H
