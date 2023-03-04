#ifndef EXPGE_EXP_SKELANIM_H
#define EXPGE_EXP_SKELANIM_H

#include <BD_AnimReader.h>

#include <vector>
#include <memory>

#include "EXP_Game.h"
#include "EXP_SkeletalMesh.h"

class EXP_SkelAnim {
public:
    EXP_SkelAnim(EXP_Game* game);
    ~EXP_SkelAnim();

    bool LoadAnimation(std::string path);

    void TransfSkelAtTime(std::shared_ptr<EXP_SkeletalMesh> skelmesh, double time);

    BD_AnimMeta GetMeta();
private:
    BD_Frame GetClosestFrame(std::vector<BD_Frame>& frames, double time, int id, bool next = false);

    EXP_Game* m_game;
    BD_AnimMeta m_meta;

    std::vector<BD_Frame> m_pos;
    std::vector<BD_Frame> m_rot;
    std::vector<BD_Frame> m_scale;
};


#endif //EXPGE_EXP_SKELANIM_H
