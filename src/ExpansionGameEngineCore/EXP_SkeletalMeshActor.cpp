#include "EXP_SkeletalMeshActor.h"

EXP_SkeletalMeshActor::EXP_SkeletalMeshActor(EXP_Game *game, std::string mesh_file, std::string skel_file,
                                             std::shared_ptr<EXP_Material> mat, vec3 pos, vec3 rot, vec3 scale) :
        EXP_Actor(pos, rot, scale)
{
    m_skel_mesh = std::make_shared<EXP_SkeletalMesh>(game, mesh_file, skel_file, mat, vec3(), vec3(), vec3(1, 1, 1));
    LinkComponent(m_skel_mesh);
}

std::shared_ptr<EXP_SkeletalMesh> EXP_SkeletalMeshActor::GetMeshComponent() {
    return m_skel_mesh;
}