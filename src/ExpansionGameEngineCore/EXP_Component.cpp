#include "EXP_Component.h"

void EXP_Component::SetParentMatrix(mat4f &matrix) {
    m_ptrans = matrix;
}

vec3 EXP_Component::GetComponentPos() {
    return m_pos;
}

void EXP_Component::SetComponentPos(vec3 npos) {
    m_pos = npos;
    UpdateTransform();
}

vec3 EXP_Component::GetComponentRot() {
    return m_rot;
}

void EXP_Component::SetComponentRot(vec3 nrot) {
    m_rot = nrot;
    UpdateTransform();
}

vec3 EXP_Component::GetComponentScale() {
    return m_scale;
}

void EXP_Component::SetComponentScale(vec3 nscale) {
    m_scale = nscale;
    UpdateTransform();
}

mat4f EXP_Component::GetComponentTransform() {
    return m_transform;
}

void EXP_Component::UpdateTransform() {
    m_transform = mat4f(1.0f);

    mat4f trans = mat4f(1.0f);
    mat4f scale = mat4f(1.0f);
    mat4f rot = mat4f(1.0f);

    trans = TranslateMatrix(trans, m_pos);
    scale = ScaleMatrix(scale, m_scale);
    rot = RotateMatrix(rot, m_rot);

    m_transform = trans * rot * scale;
    m_transform = m_ptrans * m_transform;
}