#ifndef EXPGE_EXP_BODY_H
#define EXPGE_EXP_BODY_H

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/Body.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

#include <vec.h>
#include <quat.h>
#include <mat4.h>

#include "EXP_PhysicsHandler.h"
#include "EXP_Component.h"

#include <memory>
#include <functional>

class EXP_Body : public EXP_Component {
public:
    EXP_Body(std::shared_ptr<EXP_PhysicsHandler> handler, vec3 pos, vec3 rot, vec3 scale, bool isStatic);
    virtual ~EXP_Body();

    JPH::Body* GetBody() { return m_body; }

    vec3 GetBodyPos();
    Quat GetBodyRot();
    mat4f& GetBodyTransform();

    mat4f& GetComponentTransform() override;

protected:
    void CreateBodyFromShape(JPH::BodyInterface& interface, JPH::ShapeRefC& shape, vec3 pos, vec3 rot, vec3 scale);

    std::shared_ptr<EXP_PhysicsHandler> m_handler;

    JPH::Body* m_body;
    JPH::BodyID m_id;

    bool m_isStatic;
};


#endif //EXPGE_EXP_BODY_H
