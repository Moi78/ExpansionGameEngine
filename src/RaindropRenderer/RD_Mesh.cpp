#include "RD_Mesh.h"

RD_Mesh::RD_Mesh(std::shared_ptr<RD_API> api, vec3 pos, vec3 rot, vec3 scale) {
    m_api = api;

    m_pos = std::move(pos);
    m_rot = std::move(rot);
    m_scale = std::move(scale);

    m_transf = mat4f();
    Update();
}

RD_Mesh::~RD_Mesh() {

}

void RD_Mesh::LoadMesh(const std::string& filepath) {
    BD_Reader r = BD_Reader();
    r.ReadMSHFile(filepath);

    std::vector<vec3> pos, norm;
    std::vector<vec2> uv;
    std::vector<uint32_t> indicies;

    for(int i = 0; i < r.GetVerticesCount(); i++) {
        pos.push_back(r.GetVertexByIndex(i));
        norm.push_back(r.GetNormalByIndex(i));
        uv.push_back(r.GetUVcoordByIndex(i));
    }

    for(int i = 0; i < r.GetIndicesCount(); i++) {
        indicies.push_back(r.GetIndiceByIndex(i));
    }

    m_vbuff = m_api->CreateIndexedVertexBuffer();
    auto data = MakeVertexData(pos, norm, uv);
    m_vbuff->FillBufferData(data, indicies);
}

void RD_Mesh::Update() {
    m_transf = mat4f();

    m_transf = RotateMatrix(m_transf, m_rot);
    m_transf = ScaleMatrix(m_transf, m_scale);
    m_transf = TranslateMatrix(m_transf, m_pos);
}

mat4f RD_Mesh::GetTransform() {
    return m_transf;
}

void RD_Mesh::SetTransform(mat4f &transform) {
    m_transf = transform;
}

std::shared_ptr<RD_IndexedVertexBuffer> RD_Mesh::GetVertexBuffer() {
    return m_vbuff;
}

void RD_Mesh::Rotate(vec3 rotation) {
    m_rot = m_rot + rotation;
    Update();
}