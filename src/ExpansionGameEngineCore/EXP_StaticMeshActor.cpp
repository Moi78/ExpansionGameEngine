#include "EXP_StaticMeshActor.h"

EXP_StaticMeshActor::EXP_StaticMeshActor(EXP_Game *game, std::string mesh_file, std::shared_ptr<EXP_Material> mat,
                                         vec3 pos, vec3 rot, vec3 scale) : EXP_Actor(pos, rot, scale)
{
    m_smesh_comp = std::make_shared<EXP_StaticMesh>(game, mesh_file, mat, vec3(0.0, 0.0, 0.0f), vec3(), vec3(1.0f, 1.0f, 1.0f));
    LinkComponent(m_smesh_comp);
}