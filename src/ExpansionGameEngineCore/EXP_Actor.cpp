#include "EXP_Actor.h"

EXP_Actor::EXP_Actor(vec3 pos, vec3 rot, vec3 scale) {
    m_root = std::make_shared<EXP_Component>(pos, rot, scale);

    using namespace std::chrono;
    m_uid = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
    std::cout << "Created actor (UID : " << (long long) m_uid << ")" << std::endl;
}

vec3 EXP_Actor::GetActorPos() {
    return m_root->GetComponentPos();
}

void EXP_Actor::SetActorPos(vec3 npos) {
    m_root->SetComponentPos(npos);
    UpdateActor();
}

vec3 EXP_Actor::GetActorRot() {
    return m_root->GetComponentRot();
}

void EXP_Actor::SetActorRot(vec3 nrot) {
    m_root->GetComponentRot();
    UpdateActor();
}

vec3 EXP_Actor::GetActorScale() {
    return m_root->GetComponentScale();
}

void EXP_Actor::SetActorScale(vec3 nscale) {
    m_root->SetComponentScale(nscale);
    UpdateActor();
}

mat4f EXP_Actor::GetActorTransform() {
    return m_root->GetComponentTransform();
}

void EXP_Actor::LinkComponent(std::shared_ptr<EXP_Component> comp) {
    comp->SetParentMatrix(m_root->GetComponentTransform());
    m_comps.push_back(comp);
}

void EXP_Actor::UpdateActor() {
    for(auto& c : m_comps) {
        c->SetParentMatrix(m_root->GetComponentTransform());
    }
}

uint64_t EXP_Actor::GetActorUID() {
    return m_uid;
}

void EXP_Actor::TranslateActor(vec3 trans) {
    m_root->TranslateComponent(trans);
    UpdateActor();
}

void EXP_Actor::RotateActor(vec3 rotation) {
    m_root->RotateComponent(rotation);
    UpdateActor();
}

void EXP_Actor::SetRootComponent(std::shared_ptr<EXP_Component> comp) {
    m_root = comp;
}
