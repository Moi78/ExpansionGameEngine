#include "RD_Skeleton.h"

RD_Skeleton::RD_Skeleton(size_t skel_offset) {
    m_skel_offset = skel_offset;
}

RD_Skeleton::~RD_Skeleton() {

}

void RD_Skeleton::UploadSkeleton(std::shared_ptr<RD_UniformBuffer> dest) {
    for(int i = 0; i < m_skeleton.size(); i++) {
        dest->PartialFillBufferData(m_skeleton[i].pos.GetPTR(), 16 * sizeof(float), (i * 16 * sizeof(float)) + m_skel_offset);
    }
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
}

size_t RD_Skeleton::GetBoneCount() {
    return m_skeleton.size();
}