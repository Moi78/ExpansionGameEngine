#include "EXP_StaticMesh.h"

EXP_StaticMesh::EXP_StaticMesh(EXP_Game* game, std::string mesh_file, std::shared_ptr<EXP_Material> mat, vec3 pos, vec3 rot,
                               vec3 scale) : EXP_Component(pos, rot, scale)
{
    m_mesh = std::make_shared<RD_Mesh>(game->GetRenderer()->GetAPI(), pos, rot, scale);
    m_mesh->LoadMesh(game->GetGameContentPath() + mesh_file);

    mat->RegisterMesh(m_mesh);
}

void EXP_StaticMesh::SetParentMatrix(mat4f& pmat) {
    m_mesh->SetTransform(pmat);
}