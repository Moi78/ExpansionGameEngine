#include "EXP_Camera.h"

EXP_Camera::EXP_Camera(EXP_Game *game, vec3 pos, vec3 ypr, float FOV, float near, float far) :
    EXP_Component(pos, vec3(), vec3()),
    RD_Camera(game->GetRenderer()->GetAPI(), FOV, pos, vec3(), near, far)
{
    m_ypr = ypr;
    m_target = vec3();
    ComputeYPR();
}

void EXP_Camera::ComputeYPR() {
    const float yaw = DEG_TO_RAD(m_ypr.GetX());
    const float pitch = DEG_TO_RAD(m_ypr.GetY());
    const float roll2 = DEG_TO_RAD(m_ypr.GetZ()) / 2;

    m_target.SetAll(
            cos(yaw) * cos(pitch),
            sin(yaw) * cos(pitch),
            sin(pitch)
    );
    UpdateView();

    vec3 fwd = GetForwardVector();
    const float sRoll2 = sin(roll2);

    Quat rotator(cos(roll2), sRoll2 * fwd.GetX(), sRoll2 * fwd.GetY(), sRoll2 * fwd.GetZ());
    Quat rotatorConj = rotator.GetConjugate();

    Quat vector(0.0f, 0.0f, 0.0f, 1.0f);
    Quat up_rot = (rotator * vector) * rotatorConj;
    SetUp(up_rot.GetImagPart());
}

void EXP_Camera::SetParentMatrix(mat4f &transf) {
    vec4 pos(EXP_Component::m_pos, 1.0f);
    vec4 rpos = transf * pos;

    auto p = rpos.ToVec3();
    SetPos(rpos.ToVec3());
}

void EXP_Camera::AddYaw(float y) {
    m_ypr = m_ypr + vec3(y);
    ComputeYPR();
}

void EXP_Camera::AddPitch(float p) {
    m_ypr = m_ypr + vec3(0.0f, p);
    ComputeYPR();
}

void EXP_Camera::AddRoll(float r) {
    m_ypr = m_ypr + vec3(0.0f, 0.0f, r);
    ComputeYPR();
}