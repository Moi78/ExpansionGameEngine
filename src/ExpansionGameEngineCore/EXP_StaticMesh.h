#ifndef EXPGE_EXP_STATICMESH_H
#define EXPGE_EXP_STATICMESH_H

#include <string>
#include <memory>

#include "EXP_Material.h"
#include "EXP_Component.h"

#include <RD_Mesh.h>

class EXP_StaticMesh : public EXP_Component {
public:
    EXP_StaticMesh(EXP_Game* game, std::string mesh_file, std::shared_ptr<EXP_Material> mat, vec3 pos, vec3 rot, vec3 scale);
    virtual ~EXP_StaticMesh() {};

    virtual void SetParentMatrix(mat4f& pmat) override;

    std::shared_ptr<EXP_Material> GetMeshMaterial();
    void SetMeshMaterial(std::shared_ptr<EXP_Material> mat);

private:
    std::shared_ptr<RD_Mesh> m_mesh;
    std::shared_ptr<EXP_Material> m_matref;
};


#endif //EXPGE_EXP_STATICMESH_H
