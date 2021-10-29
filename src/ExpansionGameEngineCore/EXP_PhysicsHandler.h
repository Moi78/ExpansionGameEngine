#pragma once

#ifndef _EXP_PHYSICS_HANDLER_H__
#define _EXP_PHYSICS_HANDLER_H__

#ifdef _WIN32

#ifdef EXPANSIONGAMEENGINECORE_EXPORTS
#define EXPGE_API __declspec(dllexport)
#else
#define EXPGE_API __declspec(dllimport)
#endif

#else

#define EXPGE_API

#endif //_WIN32

#include <PxPhysicsAPI.h>
#include <foundation/PxErrorCallback.h>
#include <extensions/PxDefaultAllocator.h>

#include <iostream>
#include <thread>
#include <vector>

#include <vec3.h>
#include "EXP_RigidBody.h"

class EXPGE_API EXP_PhysicsHandler
{
public:
	EXP_PhysicsHandler(vec3f gravity, int maxFramerate);
	~EXP_PhysicsHandler();

	void UpdateWorld();
	void InitWorld();

	void RegisterRigidBody(EXP_RigidBody*);
	void RemoveBodyFromWorld(EXP_RigidBody*);
	void PurgeControllers();

	physx::PxPhysics* GetPhysics() { return m_physics; }
	physx::PxCooking* GetCooker() { return m_cooker; }
	physx::PxControllerManager* GetControllerManager() { return m_controller_man; }

private:
	int GetPrefferedNumberOfThreads();

	int m_maxFramerate;
	float m_updtTime;

	vec3f m_gravity;

	physx::PxFoundation* m_fnd;
	physx::PxPvd* m_PVD;
	physx::PxPhysics* m_physics;
	physx::PxCooking* m_cooker;
	physx::PxControllerManager* m_controller_man;

	physx::PxScene* m_world;

	std::vector<EXP_RigidBody*> m_bodies;
};

#endif //_EXP_PHYSICS_HANDLER_H__