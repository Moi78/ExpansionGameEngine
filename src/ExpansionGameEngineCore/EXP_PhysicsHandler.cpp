#include "EXP_PhysicsHandler.h"

BroadPhaseLayerImpl::BroadPhaseLayerImpl() {
    m_objectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
    m_objectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
}

uint BroadPhaseLayerImpl::GetNumBroadPhaseLayers() const {
    return BroadPhaseLayers::NUM_LAYERS;
}

JPH::BroadPhaseLayer BroadPhaseLayerImpl::GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const {
    return m_objectToBroadPhase[inLayer];
}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
const char *BroadPhaseLayerImpl::GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const {
    switch((JPH::BroadPhaseLayer::Type)inLayer) {
        case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING: return "NON MOVING";
        case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:     return "MOVING";
        default:                                                       return "INVALID";
    }
}
#endif //defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)

// ---------------------------------------------------------------------------------------------------------------------

bool ObjectVsBroadPhaseLayerFilterImpl::ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const {
    switch(inLayer1) {
        case Layers::NON_MOVING:
            return inLayer2 == BroadPhaseLayers::MOVING;
        case Layers::MOVING:
            return true;
        default:
            return false;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

bool ObjectLayerPairFilterImpl::ShouldCollide(JPH::ObjectLayer inLayer1, JPH::ObjectLayer inLayer2) const {
    switch(inLayer1) {
        case Layers::NON_MOVING:
            return inLayer2 == Layers::MOVING;
        case Layers::MOVING:
            return true;
        default:
            return false;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

EXP_PhysicsHandler::EXP_PhysicsHandler() {

}

EXP_PhysicsHandler::~EXP_PhysicsHandler() {
    delete JPH::Factory::sInstance;
    JPH::Factory::sInstance = nullptr;
}

void EXP_PhysicsHandler::InitPhysics() {
    using namespace JPH;

    RegisterDefaultAllocator();

    Factory::sInstance = new Factory();
    TempAllocatorImpl tmpAlloc(10 * 1024 * 1024); // 10 MB

    JobSystemThreadPool jobSystem(cMaxPhysicsJobs, cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1);

    int maxBodies = 65536;
    int numBodyMutexes = 0;
    int maxBodyPairs = 65536;
    int maxContactConstraints = 10240;

    m_world = std::make_unique<JPH::PhysicsSystem>();
    m_world->Init(maxBodies, numBodyMutexes, maxBodyPairs, maxContactConstraints, m_bp_layer, m_obj_bp_filter, m_obj_filter);
}
