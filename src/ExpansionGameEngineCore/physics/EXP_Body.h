#ifndef EXPGE_EXP_BODY_H
#define EXPGE_EXP_BODY_H

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/Body.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

#include <vec.h>
#include <quat.h>
#include <mat4.h>

#include "EXP_PhysicsHandler.h"

#include <memory>
#include <functional>

class EXP_Body {
public:
    EXP_Body(std::shared_ptr<EXP_PhysicsHandler> handler, bool isStatic);
    virtual ~EXP_Body();

    JPH::Body* GetBody() { return m_body; }

    vec3 GetBodyPos();
    Quat GetBodyRot();
    mat4f GetBodyTransform();

protected:
    void CreateBodyFromShape(JPH::BodyInterface& interface, JPH::ShapeRefC& shape, vec3 pos, vec3 rot, vec3 scale);

    std::shared_ptr<EXP_PhysicsHandler> m_handler;

    JPH::Body* m_body;
    JPH::BodyID m_id;

    vec3 m_scale;

    bool m_isStatic;
};


#endif //EXPGE_EXP_BODY_H
