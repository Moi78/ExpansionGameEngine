#include "pch.h"
#include "EXP_PhysicsHandler.h"

EXP_PhysicsHandler::EXP_PhysicsHandler(vec3f gravity, int maxFramerate) : m_gravity(gravity), m_maxFramerate(maxFramerate) {
	m_updtTime = ((float) 1.0f / m_maxFramerate);
}

EXP_PhysicsHandler::~EXP_PhysicsHandler() {
	//Memory cleanup
	delete m_dWorld;
	delete m_solver;
	delete m_overlappingPairCache;
	delete m_dispatcher;
	delete m_CollisionConfig;
}

void EXP_PhysicsHandler::InitWorld() {
	m_CollisionConfig = new btDefaultCollisionConfiguration();

	m_dispatcher = new btCollisionDispatcher(m_CollisionConfig);

	m_overlappingPairCache = new btDbvtBroadphase();

	m_solver = new btSequentialImpulseConstraintSolver;

	m_dWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_overlappingPairCache, m_solver, m_CollisionConfig);

	m_dWorld->setGravity(btVector3(m_gravity.getX(), m_gravity.getY(), m_gravity.getZ()));
}

void EXP_PhysicsHandler::UpdateWorld() {
	m_dWorld->stepSimulation(m_updtTime, 10);
}

void EXP_PhysicsHandler::RegisterRigidBody(EXP_RigidBody* rb) {
	m_dWorld->addRigidBody(rb->GetBody());
}