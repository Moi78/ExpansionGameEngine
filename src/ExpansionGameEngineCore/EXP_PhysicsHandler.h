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

#include <vec3.h>

#include <bullet/btBulletDynamicsCommon.h>

#include "EXP_RigidBody.h"

class EXPGE_API EXP_PhysicsHandler
{
public:
	EXP_PhysicsHandler(vec3f gravity, int maxFramerate);
	~EXP_PhysicsHandler();

	void UpdateWorld();
	void InitWorld();

	void RegisterRigidBody(EXP_RigidBody*);

	void RemoveBodyFromWorld(btRigidBody*);

private:
	btDiscreteDynamicsWorld* m_dWorld;

	btCollisionDispatcher* m_dispatcher;
	btBroadphaseInterface* m_overlappingPairCache;
	btSequentialImpulseConstraintSolver* m_solver;
	btDefaultCollisionConfiguration* m_CollisionConfig;

	vec3f m_gravity;
	
	int m_maxFramerate;
	float m_updtTime;
};

#endif //_EXP_PHYSICS_HANDLER_H__