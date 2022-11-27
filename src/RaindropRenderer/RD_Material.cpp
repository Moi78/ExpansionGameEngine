#include "RD_Material.h"

RD_Material::RD_Material(std::shared_ptr<RD_Pipeline> mat) {
    m_pline = mat;
}

RD_Material::~RD_Material() {

}

void RD_Material::RegisterMesh(std::shared_ptr<RD_Mesh> mesh) {
    m_meshes.push_back(mesh);
}

void RD_Material::PurgeMeshes() {
    m_meshes.clear();
}

void RD_Material::RenderMeshes(std::shared_ptr<RD_UniformBuffer> camModel, std::shared_ptr<RD_RenderSynchronizer> sync) {
    m_pline->Bind(sync);

    for(auto& m : m_meshes) {
        //camModel->PartialFillBufferData(m->GetTransform().GetPTR(), 16 * sizeof(float), 2 * 16 * sizeof(float));
        m_pline->PushConstant(m->GetTransform().GetPTR(), 16 * sizeof(float), sync);
        m_pline->DrawIndexedVertexBuffer(m->GetVertexBuffer(), sync);
    }

    m_pline->Unbind(sync);
}

void RD_Material::RenderMeshesExtPline(std::shared_ptr<RD_Pipeline> pline, std::shared_ptr<RD_RenderSynchronizer> sync) {
    for(auto& m : m_meshes) {
        pline->PushConstant(m->GetTransform().GetPTR(), 16 * sizeof(float), sync);
        pline->DrawIndexedVertexBuffer(m->GetVertexBuffer(), sync);
    }
}