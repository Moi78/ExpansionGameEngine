#include "EXP_SkelAnim.h"

EXP_SkelAnim::EXP_SkelAnim(EXP_Game* game) {
    m_game = game;
}

EXP_SkelAnim::~EXP_SkelAnim() {

}

bool EXP_SkelAnim::LoadAnimation(std::string path) {
    BD_AnimReader ar;
    if(!ar.ReadFile(m_game->GetGameContentPath() + path)) {
        return false;
    }

    for(int i = 0; i < ar.GetFrameCount(); i++) {
        BD_Frame f = ar.GetFrameByIndex(i);
        if(f.chanType == BD_CHAN_POS) {
            m_pos.push_back(f);
        } else if(f.chanType == BD_CHAN_SCALE) {
            m_scale.push_back(f);
        } else if(f.chanType == BD_CHAN_ROT) {
            m_rot.push_back(f);
        }
    }

    m_meta = ar.GetMeta();

    return true;
}

void EXP_SkelAnim::TransfSkelAtTime(std::shared_ptr<EXP_SkeletalMesh> skelmesh, double time) {
    std::shared_ptr<RD_Skeleton> skel = skelmesh->GetSkeleton();

    for(int b = 0; b < skel->GetBoneCount(); b++) {
        mat4f transf = mat4f(1.0f);

        BD_Frame closest_pos = GetClosestFrame(m_pos, time, b);
        vec3 pos = vec3(closest_pos.vecData[0], closest_pos.vecData[1], closest_pos.vecData[2]);
        transf = TranslateMatrix(transf, pos);

        BD_Frame closest_rot = GetClosestFrame(m_rot, time, b);
        Quat rot = Quat(closest_rot.vecData[3], closest_rot.vecData[0], closest_rot.vecData[1], closest_rot.vecData[2]);
        transf = transf * rot.ToMat4();
        rot.ToMat4().DBG_print_matrix();

        BD_Frame closest_scale = GetClosestFrame(m_scale, time, b);
        vec3 scale = vec3(closest_scale.vecData[0], closest_scale.vecData[1], closest_scale.vecData[2]);
        transf = ScaleMatrix(transf, vec3(1.0f, 1.0f, 1.0f));

        skel->TransformBone(transf, b);
    }

    m_game->UpdateSkeleton(skel);
}

BD_Frame EXP_SkelAnim::GetClosestFrame(std::vector<BD_Frame> &frames, double time, int id, bool next) {
    std::vector<BD_Frame> chanFrames;
    for(auto& f : frames) {
        if(f.boneID == id) {
            chanFrames.push_back(f);
        }
    }

    if(chanFrames.size() == 0) {
        return {BD_CHAN_EMPTY, 0, {0, 0, 0, 0}, 0};
    }

    BD_Frame closest = chanFrames[0];
    double frameNb = time;
    for(auto& f : chanFrames) {
        if(next) {
            if(f.time > frameNb) {
                if(abs(f.time - frameNb) < abs(closest.time - frameNb)) {
                    closest = f;
                }
            } else {
                continue;
            }
        } else {
            if(f.time <= frameNb) {
                if(abs(f.time - frameNb) < abs(closest.time - frameNb)) {
                    closest = f;
                }
            } else {
                continue;
            }
        }
    }

    return closest;
}
