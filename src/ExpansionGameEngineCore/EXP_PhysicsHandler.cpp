#include "pch.h"
#include "EXP_PhysicsHandler.h"

EXP_PhysicsHandler::EXP_PhysicsHandler(vec3f gravity, int maxFramerate) : m_gravity(gravity), m_maxFramerate(maxFramerate) {
	m_updtTime = ((float) 1.0f / m_maxFramerate);
}

EXP_PhysicsHandler::~EXP_PhysicsHandler() {
	PxCloseExtensions();
	m_physics->release();
	m_PVD->release();
	m_cooker->release();
	m_fnd->release();
	//m_controller_man->release();

	std::cout << "Shutted down physics engine" << std::endl;
}

void EXP_PhysicsHandler::InitWorld() {
    std::cout << "Initializing Physics Engine" << std::endl;
    
	static physx::PxDefaultErrorCallback m_errCallback;
	static physx::PxDefaultAllocator m_allocCallback;

	m_fnd = PxCreateFoundation(PX_PHYSICS_VERSION, m_allocCallback, m_errCallback);
	if (!m_fnd) {
		dispErrorMessageBox(StrToWStr("Failed to create PxFoundation !"));
		return;
	}
	else {
		std::cout << "Created PxFoundation." << std::endl;
	}

	m_PVD = physx::PxCreatePvd(*m_fnd);
	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("localhost", 5425, 10);
	m_PVD->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

	m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_fnd, physx::PxTolerancesScale(), true, m_PVD);
	if (!m_physics) {
		dispErrorMessageBox(StrToWStr("Failed to create PxPhysics !"));
		return;
	}
	else {
		std::cout << "Created PxPhysics." << std::endl;
	}

	m_cooker = PxCreateCooking(PX_PHYSICS_VERSION, *m_fnd, physx::PxCookingParams(physx::PxTolerancesScale()));
	if (!m_cooker) {
		dispErrorMessageBox(StrToWStr("Failed to create PxCooking !"));
		return;
	}
	else {
		std::cout << "Created PxCooking." << std::endl;
	}

	if (!PxInitExtensions(*m_physics, m_PVD)) {
		dispErrorMessageBox(StrToWStr("PxInitExtension failed !"));
		return;
	}
	else {
		std::cout << "Initialized Extensions." << std::endl;
	}

	physx::PxSceneDesc desc = physx::PxSceneDesc(physx::PxTolerancesScale());
	desc.gravity = physx::PxVec3(m_gravity.getX(), m_gravity.getY(), m_gravity.getZ());
	desc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(GetPrefferedNumberOfThreads());
	desc.filterShader = physx::PxDefaultSimulationFilterShader;

	m_world = m_physics->createScene(desc);
	if (!m_world) {
		dispErrorMessageBox(StrToWStr("Failed to create PxScene."));
		return;
	}
	else {
		std::cout << "Created PxScene." << std::endl;
	}

	//m_controller_man = PxCreateControllerManager(*m_world);
	if (!m_controller_man) {
		dispErrorMessageBox(StrToWStr("Failed to create PxControllerManager."));
		return;
	}
	else {
		std::cout << "Created PxControllerManager." << std::endl;
	}
}

void EXP_PhysicsHandler::UpdateWorld() {
	m_world->simulate(1.0f / 60.0f);
	m_world->fetchResults(true);
}

void EXP_PhysicsHandler::RegisterRigidBody(EXP_RigidBody* rb) {
	std::cout << "Registering new rigid body." << std::endl;
	m_world->addActor(*rb->GetBody());
	m_bodies.push_back(rb);
}

void EXP_PhysicsHandler::RemoveBodyFromWorld(EXP_RigidBody* bd) {
	m_world->removeActor(*bd->GetBody());

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
	//m_controller_man->purgeControllers();
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