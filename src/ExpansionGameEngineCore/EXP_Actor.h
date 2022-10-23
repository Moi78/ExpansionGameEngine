#ifndef EXPGE_EXP_ACTOR_H
#define EXPGE_EXP_ACTOR_H

#include <memory>
#include <vector>
#include <chrono>
#include <cstddef>

#include <vec.h>
#include <mat4.h>

#include "EXP_Component.h"

class EXP_Actor {
public:
    EXP_Actor(vec3 pos, vec3 rot, vec3 scale);

    virtual ~EXP_Actor() {}

    vec3 GetActorPos();
    vec3 GetActorRot();
    vec3 GetActorScale();

    uint64_t GetActorUID();

    void SetActorPos(vec3 npos);
    void SetActorRot(vec3 nrot);
    void SetActorScale(vec3 nscale);

    mat4f GetActorTransform();
    void UpdateTransform();
    void UpdateActor();

    virtual void OnStart() = 0;
    virtual void OnTick() = 0;

    void LinkComponent(std::shared_ptr<EXP_Component> comp);
protected:
    vec3 m_pos, m_rot, m_scale;
    mat4f m_transform;

    std::vector<std::shared_ptr<EXP_Component>> m_comps;

    uint64_t m_uid;
};


#endif //EXPGE_EXP_ACTOR_H
