#include "EXP_Actor.h"

EXP_Actor::EXP_Actor(vec3 pos, vec3 rot, vec3 scale) {
    m_pos = pos;
    m_rot = rot;
    m_scale = scale;

    UpdateTransform();

    using namespace std::chrono;
    m_uid = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
    std::cout << "Created actor (UID : " << (long long) m_uid << ")" << std::endl;
}

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

    mat4f trans = mat4f(1.0f);
    mat4f scale = mat4f(1.0f);
    mat4f rot = mat4f(1.0f);

    trans = TranslateMatrix(trans, m_pos);
    scale = ScaleMatrix(scale, m_scale);
    rot = RotateMatrix(rot, m_rot);

    m_transform = trans * rot * scale;

    UpdateActor();
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

uint64_t EXP_Actor::GetActorUID() {
    return m_uid;
}

void EXP_Actor::TranslateActor(vec3 trans) {
    m_pos = m_pos + trans;
    UpdateTransform();
}

void EXP_Actor::RotateActor(vec3 rotation) {
    m_rot = m_rot + rotation;
    UpdateTransform();
}