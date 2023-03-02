#ifndef EXPGE_EXP_SKELETALMESH_H
#define EXPGE_EXP_SKELETALMESH_H

#include <RD_Skeleton.h>
#include <RD_Mesh.h>

#include "EXP_Game.h"
#include "EXP_Component.h"
#include "EXP_Material.h"

class EXP_SkeletalMesh : public EXP_Component {
public:
    EXP_SkeletalMesh(EXP_Game* game, std::string mesh_file, std::string skel_file, std::shared_ptr<EXP_Material> mat, vec3 pos, vec3 rot, vec3 scale);
    virtual ~EXP_SkeletalMesh();

    virtual void SetParentMatrix(mat4f& pmat) override;

    std::shared_ptr<RD_Skeleton> GetSkeleton();

private:
    std::shared_ptr<RD_Mesh> m_mesh;
};


#endif //EXPGE_EXP_SKELETALMESH_H
