#include "EXP_Body.h"

EXP_Body::EXP_Body(std::shared_ptr<EXP_PhysicsHandler> handler, vec3 pos, vec3 rot, vec3 scale, bool isStatic) :
EXP_Component(pos, rot, scale) {
    m_handler = handler;
    m_isStatic = isStatic;
}

EXP_Body::~EXP_Body() {

}

void EXP_Body::CreateBodyFromShape(JPH::BodyInterface& interface, JPH::ShapeRefC &shape, vec3 pos, vec3 rot, vec3 scale) {
    m_scale = scale;

    JPH::EMotionType motionType = JPH::EMotionType::Dynamic;
    JPH::ObjectLayer layerType = Layers::MOVING;

    if(m_isStatic) {
        motionType = JPH::EMotionType::Static;
        layerType = Layers::NON_MOVING;
    }

    Quat qRot = Quat::FromEuler(rot);
    JPH::BodyCreationSettings bodyCreationSettings(
            shape,
            JPH::Vec3(pos.GetX(), pos.GetY(), pos.GetZ()),
            JPH::Quat(qRot.GetX(), qRot.GetY(), qRot.GetZ(), qRot.GetW()),
            motionType,
            layerType
    );

    m_body = interface.CreateBody(bodyCreationSettings);
    m_id = m_body->GetID();

    interface.AddBody(m_body->GetID(), m_isStatic ? JPH::EActivation::DontActivate : JPH::EActivation::Activate);
}

vec3 EXP_Body::GetBodyPos() {
    static JPH::BodyInterface& bodyInterface = m_handler->GetBodyInterface();

    JPH::Vec3 pos = bodyInterface.GetCenterOfMassPosition(m_id);
    return vec3(pos.GetX(), pos.GetY(), pos.GetZ());
}

Quat EXP_Body::GetBodyRot() {
    static JPH::BodyInterface& bodyInterface = m_handler->GetBodyInterface();

    JPH::Quat rot = bodyInterface.GetRotation(m_id);
    return Quat(-rot.GetW(), rot.GetX(), rot.GetY(), -rot.GetZ());
}

mat4f& EXP_Body::GetBodyTransform() {
    m_pos = GetBodyPos();

    m_transform = TranslateMatrix(mat4f(1.0f), GetBodyPos());
    m_transform = m_transform * GetBodyRot().ToMat4();
    m_transform = ScaleMatrix(m_transform, m_scale);

    return m_transform;
}

mat4f& EXP_Body::GetComponentTransform() {
    return GetBodyTransform();
}
