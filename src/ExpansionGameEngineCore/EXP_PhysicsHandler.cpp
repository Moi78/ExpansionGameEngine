#include "pch.h"
#include "EXP_PhysicsHandler.h"

EXP_PhysicsHandler::EXP_PhysicsHandler(vec3f gravity, int maxFramerate) : m_gravity(gravity), m_maxFramerate(maxFramerate) {
	m_updtTime = ((float) 1.0f / m_maxFramerate);
}

EXP_PhysicsHandler::~EXP_PhysicsHandler() {
	delete m_world;

	std::cout << "Shutted down physics engine" << std::endl;
}

void EXP_PhysicsHandler::InitWorld() {
    std::cout << "Initializing Physics Engine" << std::endl;
    
	btDefaultCollisionConfiguration* collConf = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collConf);
	btBroadphaseInterface* overlappingCache = new btDbvtBroadphase();
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	m_world = new btDiscreteDynamicsWorld(dispatcher, overlappingCache, solver, collConf);
	m_world->setGravity(btVector3(m_gravity.getX(), m_gravity.getY(), m_gravity.getZ()));
}

void EXP_PhysicsHandler::UpdateWorld() {
	m_world->stepSimulation(m_updtTime);
	m_world->performDiscreteCollisionDetection();
}

void EXP_PhysicsHandler::RegisterRigidBody(EXP_RigidBody* rb) {
	std::cout << "Registering new rigid body." << std::endl;
	
	m_world->addRigidBody(rb->GetBody());

	m_bodies.push_back(rb);
}

void EXP_PhysicsHandler::RemoveBodyFromWorld(EXP_RigidBody* bd) {
	m_world->removeRigidBody(bd->GetBody());

	const int index = GetElemIndex<EXP_RigidBody*>(m_bodies, bd);

	if (index != -1) {
		m_bodies.erase(m_bodies.begin() + index);
	}
	else {
		std::cerr << "ERROR: Element does not exists" << std::endl;
	}

	delete bd;
}

void EXP_PhysicsHandler::PurgeControllers() {
	
}

int EXP_PhysicsHandler::GetPrefferedNumberOfThreads() {
	const int nbrThreads = std::thread::hardware_concurrency();
	std::cout << "Detected " << nbrThreads << " CPU threads." << std::endl;
	
	if (nbrThreads == 0) {
		std::cerr << "ERROR: Cannot get number of available CPU threads. Using 2 threads for the physics engine." << std::endl;
		return 2;
	}

	if (nbrThreads <= 4) {
		std::cout << "Using 2 threads for physics engine." << std::endl;
		return 2;
	}

	if (nbrThreads > 4) {
		std::cout << "Using 4 threads for physics engine." << std::endl;
		return 4;
	}
}