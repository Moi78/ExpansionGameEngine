#include "RD_Mesh.h"

RD_Mesh::RD_Mesh(std::shared_ptr<RD_API> api, vec3 pos, vec3 rot, vec3 scale) {
    m_api = api;

    m_pos = std::move(pos);
    m_rot = std::move(rot);
    m_scale = std::move(scale);

    m_hasSkel = false;

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
    std::vector<vec4> weights;
    std::vector<vec4> bonesID;
    std::vector<uint32_t> indicies;
    std::vector<vec3> tan, bitan;

    for(int i = 0; i < r.GetVerticesCount(); i++) {
        pos.push_back(r.GetVertexByIndex(i));
        norm.push_back(r.GetNormalByIndex(i));
        uv.push_back(r.GetUVcoordByIndex(i));
    }

    for(int i = 0; i < r.GetIndicesCount(); i++) {
        indicies.push_back(r.GetIndiceByIndex(i));
    }

    for(int i = 0; i < r.GetVertexWeightCount(); i++) {
        weights.push_back(r.GetVertexWeightByIndex(i));
        bonesID.push_back(r.GetBoneIDByIndex(i));
    }

    tan.resize(pos.size());
    bitan.resize(pos.size());

    m_vbuff = m_api->CreateIndexedVertexBuffer();
    auto data = MakeVertexData(
            pos,
            norm,
            uv,
            tan,
            bitan,
            weights,
            bonesID
    );

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

bool RD_Mesh::HasSkeleton() {
    return m_hasSkel;
}

std::shared_ptr<RD_Skeleton> RD_Mesh::GetSkeleton() {
    assert(m_hasSkel);

    return m_skel;
}

void RD_Mesh::SetSkeleton(std::shared_ptr<RD_Skeleton> skel) {
    m_skel = skel;
    m_hasSkel = true;
}