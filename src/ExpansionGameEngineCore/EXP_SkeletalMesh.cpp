#include "EXP_SkeletalMesh.h"

EXP_SkeletalMesh::EXP_SkeletalMesh(EXP_Game *game, std::string mesh_file, std::string skel_file,
                                   std::shared_ptr<EXP_Material> mat, vec3 pos, vec3 rot, vec3 scale) :
        EXP_Component(pos, rot, scale)
{
    m_mesh = std::make_shared<RD_Mesh>(game->GetRenderer()->GetAPI(), pos, rot, scale);
    m_mesh->LoadMesh(game->GetGameContentPath() + mesh_file);

    std::shared_ptr<RD_Skeleton> skel = std::make_shared<RD_Skeleton>(0);
    skel->ReadSkeleton(game->GetGameContentPath() + skel_file);

    game->SetupSkeleton(skel);
    m_mesh->SetSkeleton(skel);

    mat->RegisterMesh(m_mesh);
}

EXP_SkeletalMesh::~EXP_SkeletalMesh() {

}

void EXP_SkeletalMesh::SetParentMatrix(mat4f &pmat) {
    mat4f compTransf = GetComponentTransform();
    mat4f realTransf = pmat * compTransf;

    m_mesh->SetTransform(realTransf);
}

std::shared_ptr<RD_Skeleton> EXP_SkeletalMesh::GetSkeleton() {
    return m_mesh->GetSkeleton();
}