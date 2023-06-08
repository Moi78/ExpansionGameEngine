#include "RD_Skeleton.h"

RD_Skeleton::RD_Skeleton(size_t skel_offset) {
    m_skel_offset = skel_offset;
}

RD_Skeleton::~RD_Skeleton() {

}

void RD_Skeleton::UploadSkeleton(std::shared_ptr<RD_UniformBuffer> dest) {
    //for(int i = 0; i < m_final_matrices.size(); i++) {
    //    dest->PartialFillBufferData(m_final_matrices[i].GetPTR(), 16 * sizeof(float), (i * 16 * sizeof(float)) + m_skel_offset);
    //}

    dest->PartialFillBufferData(m_final_matrices.data(), m_final_matrices.size() * 16 * sizeof(float), m_skel_offset);
}

size_t RD_Skeleton::GetOffset() {
    return m_skel_offset;
}

void RD_Skeleton::ReadSkeleton(std::string fpath) {
    BD_SkelReader sk_r;
    if(!sk_r.ReadFile(fpath)) {
        return;
    }

    for(int i = 0; i < sk_r.GetBoneCount(); i++) {
        m_skeleton.push_back(sk_r.GetBoneByIndex(i));
    }

    assert(m_skeleton.size() != 0 && "Skeleton can't be empty");
    ComputeFinalMatrices();
}

size_t RD_Skeleton::GetBoneCount() {
    return m_skeleton.size();
}

void RD_Skeleton::ComputeFinalMatrices() {
    m_final_matrices.resize(m_skeleton.size());

    for(int i = 0; i < m_skeleton.size(); i++) {
        m_final_matrices[i] = ResolveMatrix(m_skeleton[i]);
    }
}

mat4f RD_Skeleton::ResolveMatrix(Bone b) {
    Bone currentBone = b;

    mat4f globalTransf = b.pos * b.offset;
    while(currentBone.parent_id != -1) {
        globalTransf = m_skeleton[currentBone.parent_id].pos * globalTransf;
        currentBone = m_skeleton[currentBone.parent_id];
    }

    return globalTransf;
}

void RD_Skeleton::SetBoneTransform(mat4f transform, int idx) {
    //m_skeleton[idx].pos = m_skeleton[idx].pos * transform;
    m_skeleton[idx].pos = transform;
    ComputeFinalMatrices();
}