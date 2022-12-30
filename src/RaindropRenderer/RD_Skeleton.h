#ifndef EXPGE_RD_SKELETON_H
#define EXPGE_RD_SKELETON_H

#include <BD_SkelReader.h>

#include "RD_UniformBuffer.h"

class RD_Skeleton {
public:
    RD_Skeleton(size_t skel_offset);
    ~RD_Skeleton();

    void UploadSkeleton(std::shared_ptr<RD_UniformBuffer> dest);
    size_t GetOffset();

    void ReadSkeleton(std::string fpath);

    size_t GetBoneCount();

private:
    std::vector<Bone> m_skeleton;
    size_t m_skel_offset;
};

#endif //EXPGE_RD_SKELETON_H
