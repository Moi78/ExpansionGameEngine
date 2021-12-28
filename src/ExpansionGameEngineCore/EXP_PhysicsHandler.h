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

#include <iostream>
#include <thread>
#include <vector>

#include <vec3.h>
#include "EXP_RigidBody.h"

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

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

	btDiscreteDynamicsWorld* GetWorld() {
		return m_world;
	}

private:
	int GetPrefferedNumberOfThreads();

	int m_maxFramerate;
	float m_updtTime;

	vec3f m_gravity;

	btDiscreteDynamicsWorld* m_world;

	std::vector<EXP_RigidBody*> m_bodies;
};

#endif //_EXP_PHYSICS_HANDLER_H__