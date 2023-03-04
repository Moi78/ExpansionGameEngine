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

    std::cout << "Loading animation " << path << std::endl;

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
        mat4f transM = mat4f(1.0f);
        BD_Frame closest_pos = GetClosestFrame(m_pos, time, b);

        if(closest_pos.time == time) {
            BD_Frame closest_pos_next = GetClosestFrame(m_pos, time, b, true);

            float lerpPos = (time - closest_pos.time) / (closest_pos_next.time - closest_pos.time);
            lerpPos = std::clamp(lerpPos, 0.0f, 1.0f);

            vec3 pos_a = vec3(closest_pos.vecData[0], closest_pos.vecData[1], closest_pos.vecData[2]);
            vec3 pos_b = vec3(closest_pos_next.vecData[0], closest_pos_next.vecData[1], closest_pos_next.vecData[2]);
            transM = TranslateMatrix(transM, Lerp(pos_a, pos_b, lerpPos));
        } else {
            vec3 pos_a = vec3(closest_pos.vecData[0], closest_pos.vecData[1], closest_pos.vecData[2]);
            transM = TranslateMatrix(transM, pos_a);
        }

        mat4f rotM;
        BD_Frame closest_rot = GetClosestFrame(m_rot, time, b);
        if(closest_rot.time == time) {
            BD_Frame closest_rot_next = GetClosestFrame(m_rot, time, b, true);

            float slerpPos = (time - closest_rot.time) / (closest_rot_next.time - closest_rot.time);
            slerpPos = std::clamp(slerpPos, 0.0f, 1.0f);

            Quat rot_a = Quat(closest_rot.vecData[3], closest_rot.vecData[0], closest_rot.vecData[1],
                              closest_rot.vecData[2]);
            Quat rot_b = Quat(closest_rot_next.vecData[3], closest_rot_next.vecData[0], closest_rot_next.vecData[1],
                              closest_rot_next.vecData[2]);
            Quat rot = Slerp(rot_a, rot_b, slerpPos);
            rotM = rot.GetConjugate().ToMat4();
        } else {
            Quat rot_a = Quat(closest_rot.vecData[3], closest_rot.vecData[0], closest_rot.vecData[1],
                              closest_rot.vecData[2]);
            rotM = rot_a.GetConjugate().ToMat4(); // GetConjugate() to intervert Y and Z axis since Z axis is up in EXPGE
        }

        mat4f scaleM = mat4f(1.0f);
        BD_Frame closest_scale = GetClosestFrame(m_scale, time, b);

        if(closest_scale.time == time) {
            vec3 scale = vec3(closest_scale.vecData[0], closest_scale.vecData[1], closest_scale.vecData[2]);
            scaleM = ScaleMatrix(scaleM, scale);
        } else {
            BD_Frame closest_scale_next = GetClosestFrame(m_scale, time, b, true);

            float lerpPos = (time - closest_scale.time) / (closest_scale_next.time - closest_scale.time);
            lerpPos = std::clamp(lerpPos, 0.0f, 1.0f);

            vec3 scale_a = vec3(closest_scale.vecData[0], closest_scale.vecData[1], closest_scale.vecData[2]);
            vec3 scale_b = vec3(closest_scale_next.vecData[0], closest_scale_next.vecData[1], closest_scale_next.vecData[2]);
            scaleM = ScaleMatrix(scaleM, Lerp(scale_a, scale_b, lerpPos));
        }

        mat4f transf = transM * rotM * scaleM;
        skel->SetBoneTransform(transf, b);
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

BD_AnimMeta EXP_SkelAnim::GetMeta() {
    return m_meta;
}
