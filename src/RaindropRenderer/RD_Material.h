#ifndef EXPGE_RD_MATERIAL_H
#define EXPGE_RD_MATERIAL_H

#include <vector>
#include <memory>

#include "RD_Mesh.h"
#include "RD_Pipeline.h"
#include "RD_UniformBuffer.h"

class RD_Material {
public:
    RD_Material(std::shared_ptr<RD_Pipeline> mat);
    ~RD_Material();

    void RegisterMesh(std::shared_ptr<RD_Mesh> mesh);
    void PurgeMeshes();

    void RenderMeshes(std::shared_ptr<RD_UniformBuffer> camModel);

private:
    std::vector<std::shared_ptr<RD_Mesh>> m_meshes;
    std::shared_ptr<RD_Pipeline> m_pline;
};


#endif //EXPGE_RD_MATERIAL_H
