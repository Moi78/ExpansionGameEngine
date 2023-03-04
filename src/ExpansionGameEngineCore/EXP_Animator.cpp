#include "EXP_Animator.h"

EXP_Animator::EXP_Animator() : EXP_GenericRessourceManager<std::shared_ptr<EXP_AnimTracker>>() {
    m_last_updt_stamp = NowMs();
}

EXP_Animator::~EXP_Animator() {

}

bool EXP_Animator::PlayAnimation(std::shared_ptr<EXP_SkelAnim> anim, std::shared_ptr<EXP_SkeletalMesh> mesh,
                                 std::string animName, bool isLooping) {
    if(DoIDExists(animName)) {
        std::cerr << "ERROR: Anim name already exists" << std::endl;
        return false;
    }

    AddRessource(std::make_shared<EXP_AnimTracker>(anim, mesh, 1, isLooping), animName);

    return true;
}

void EXP_Animator::PauseAnimation(std::string animName) {
    if(!DoIDExists(animName)) {
        std::cerr << "ERROR: Anim " << animName << " does not exists." << std::endl;
        return;
    }

    GetRessource(animName)->isPaused = true;
}

void EXP_Animator::ResumeAnimation(std::string animName) {
    if(!DoIDExists(animName)) {
        std::cerr << "ERROR: Anim " << animName << " does not exists." << std::endl;
        return;
    }

    GetRessource(animName)->isPaused = false;
}

void EXP_Animator::StopAnimation(std::string animName) {
    if(!DoIDExists(animName)) {
        std::cerr << "ERROR: Anim " << animName << " does not exists." << std::endl;
        return;
    }

    RemoveRessource(animName);
}

void EXP_Animator::UpdateAnimations() {
    float now = NowMs();

    std::vector<std::string> toYeet;
    for(auto& anims : m_ressources) {
        auto tracker = anims.second;

        if(!tracker->isPaused) {
            tracker->anim->TransfSkelAtTime(tracker->mesh, tracker->currentFrame);

            float fps = tracker->anim->GetMeta().framerate;
            tracker->currentFrame += ((now - m_last_updt_stamp) / 1000.0f) * tracker->timeScale * fps;

            if (tracker->currentFrame > tracker->anim->GetMeta().duration) {
                if (tracker->isLooping) {
                    tracker->currentFrame = 1;
                } else {
                    toYeet.push_back(anims.first);
                }
            }
        }
    }

    for(auto& y : toYeet) {
        std::cout << "YEET" << std::endl;
        RemoveRessource(y);
    }

    m_last_updt_stamp = now;
}

EXP_AnimTracker::EXP_AnimTracker(std::shared_ptr<EXP_SkelAnim> canim, std::shared_ptr<EXP_SkeletalMesh> cmesh,
                                 float time_scale, bool b) {
    anim = canim;
    mesh = cmesh;

    currentFrame = 1;
    timeScale = time_scale;
    isLooping = b;

    isPaused = false;
}
