#ifndef EXPGE_EXP_ACTOR_H
#define EXPGE_EXP_ACTOR_H

#include <memory>
#include <vector>

#include <vec.h>
#include <mat4.h>

#include "EXP_Component.h"

class EXP_Actor {
public:
    EXP_Actor(vec3 pos, vec3 rot, vec3 scale) {
        m_pos = pos;
        m_rot = rot;
        m_scale = scale;

        UpdateTransform();
    }

    virtual ~EXP_Actor() {}

    vec3 GetActorPos();
    vec3 GetActorRot();
    vec3 GetActorScale();

    void SetActorPos(vec3 npos);
    void SetActorRot(vec3 nrot);
    void SetActorScale(vec3 nscale);

    mat4f GetActorTransform();
    void UpdateTransform();
    void UpdateActor();

    virtual void OnStart() = 0;
    virtual void OnTick() = 0;

    void LinkComponent(std::shared_ptr<EXP_Component> comp);
private:
    vec3 m_pos, m_rot, m_scale;
    mat4f m_transform;

    std::vector<std::shared_ptr<EXP_Component>> m_comps;
};


#endif //EXPGE_EXP_ACTOR_H
