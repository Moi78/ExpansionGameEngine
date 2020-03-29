#include "pch.h"
#include "EXP_PhysicsHandler.h"

EXP_PhysicsHandler::EXP_PhysicsHandler(vec3f gravity, int maxFramerate) : m_gravity(gravity), m_maxFramerate(maxFramerate) {
	m_updtTime = ((float) 1.0f / m_maxFramerate);
}

EXP_PhysicsHandler::~EXP_PhysicsHandler() {

}

void EXP_PhysicsHandler::InitWorld() {
	btDefaultCollisionConfiguration* Collisionconfig = new btDefaultCollisionConfiguration();

	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(Collisionconfig);

	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	m_dWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, Collisionconfig);

	m_dWorld->setGravity(btVector3(m_gravity.getX(), m_gravity.getY(), m_gravity.getZ()));
}

void EXP_PhysicsHandler::UpdateWorld() {
	m_dWorld->stepSimulation(m_updtTime, 10);
}

void EXP_PhysicsHandler::RegisterRigidBody(EXP_RigidBody* rb) {
	m_dWorld->addRigidBody(rb->GetBody());
}