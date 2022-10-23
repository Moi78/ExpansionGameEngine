#include "EXP_Actor.h"

vec3 EXP_Actor::GetActorPos() {
    return m_pos;
}

void EXP_Actor::SetActorPos(vec3 npos) {
    m_pos = npos;
    UpdateTransform();
}

vec3 EXP_Actor::GetActorRot() {
    return m_rot;
}

void EXP_Actor::SetActorRot(vec3 nrot) {
    m_rot = nrot;
    UpdateTransform();
}

vec3 EXP_Actor::GetActorScale() {
    return m_scale;
}

void EXP_Actor::SetActorScale(vec3 nscale) {
    m_scale = nscale;
    UpdateTransform();
}

mat4f EXP_Actor::GetActorTransform() {
    return m_transform;
}

void EXP_Actor::UpdateTransform() {
    m_transform = mat4f(1.0f);

    m_transform = TranslateMatrix(m_transform, m_pos);
    m_transform = ScaleMatrix(m_transform, m_scale);
    m_transform = RotateMatrix(m_transform, m_rot);
}

void EXP_Actor::LinkComponent(std::shared_ptr<EXP_Component> comp) {
    comp->SetParentMatrix(m_transform);
    m_comps.push_back(comp);
}

void EXP_Actor::UpdateActor() {
    for(auto& c : m_comps) {
        c->SetParentMatrix(m_transform);
    }
}