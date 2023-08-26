#ifndef EXPGE_EXP_PHYSICSHANDLER_H
#define EXPGE_EXP_PHYSICSHANDLER_H

#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>

#include <iostream>
#include <thread>
#include <memory>

namespace Layers {
    static constexpr JPH::ObjectLayer NON_MOVING = 0;
    static constexpr JPH::ObjectLayer MOVING = 1;
    static constexpr JPH::ObjectLayer NUM_LAYERS = 2;
}

namespace BroadPhaseLayers {
    static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
    static constexpr JPH::BroadPhaseLayer MOVING(1);
    static constexpr uint NUM_LAYERS(2);
}

class BroadPhaseLayerImpl : public JPH::BroadPhaseLayerInterface {
public:
    BroadPhaseLayerImpl();
    ~BroadPhaseLayerImpl() = default;

    uint GetNumBroadPhaseLayers() const override;
    JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override;

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
    const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override;
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

private:
    JPH::BroadPhaseLayer m_objectToBroadPhase[Layers::NUM_LAYERS];
};

class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter {
    bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override;
};

class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter {
    bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::ObjectLayer inLayer2) const override;
};

class EXP_PhysicsHandler {
public:
    EXP_PhysicsHandler();
    ~EXP_PhysicsHandler();

    void InitPhysics();
    void UpdatePhysics(float deltaTime);

    JPH::BodyInterface& GetBodyInterface() { return m_world->GetBodyInterface(); }
private:
    std::unique_ptr<JPH::PhysicsSystem> m_world;

    BroadPhaseLayerImpl m_bp_layer;

    ObjectVsBroadPhaseLayerFilterImpl m_obj_bp_filter;
    ObjectLayerPairFilterImpl m_obj_filter;

    std::unique_ptr<JPH::TempAllocatorImpl> m_tmpAlloc;
    std::unique_ptr<JPH::JobSystemThreadPool> m_jobSystem;
};

#endif //EXPGE_EXP_PHYSICSHANDLER_H
