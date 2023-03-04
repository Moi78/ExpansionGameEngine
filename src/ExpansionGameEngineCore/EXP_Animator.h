#ifndef EXPGE_EXP_ANIMATOR_H
#define EXPGE_EXP_ANIMATOR_H

#include "EXP_GenericRessourceManager.h"
#include "EXP_SkelAnim.h"

#include <utility>
#include <chrono>

using namespace std::chrono;
using namespace std::chrono_literals;

struct EXP_AnimTracker {
    EXP_AnimTracker(std::shared_ptr<EXP_SkelAnim> canim, std::shared_ptr<EXP_SkeletalMesh> cmesh, float time_scale, bool b);

    std::shared_ptr<EXP_SkelAnim> anim;
    std::shared_ptr<EXP_SkeletalMesh> mesh;

    float currentFrame;
    float timeScale;
    bool isLooping;
};

class EXP_Animator : private EXP_GenericRessourceManager<std::shared_ptr<EXP_AnimTracker>> {
public:
    EXP_Animator();
    ~EXP_Animator();

    bool PlayAnimation(std::shared_ptr<EXP_SkelAnim> anim, std::shared_ptr<EXP_SkeletalMesh> mesh, std::string animName, bool isLooping);
    void PauseAnimation(std::string animName);
    void StopAnimation(std::string animName);
    void ResumeAnimation(std::string animName);

    void UpdateAnimations();

private:
    float m_last_updt_stamp;
};

static float NowMs() {
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

#endif //EXPGE_EXP_ANIMATOR_H
