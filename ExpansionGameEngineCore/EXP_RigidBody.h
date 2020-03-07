#pragma once

#ifndef _EXP_RIGID_BODY_H__
#define _EXP_RIGID_BODY_H__

#ifdef EXPANSIONGAMEENGINECORE_EXPORTS
#define EXPGE_API __declspec(dllexport)
#else
#define EXPGE_API __declspec(dllimport)
#endif

#include <iostream>

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include <glm/glm.hpp>

#include <BD_StructMan.h>

#include "EXP_Game.h"

class EXPGE_API EXP_RigidBody
{
public:
	EXP_RigidBody(EXP_Game* game, BD_RBodyShape shape, float mass);
	~EXP_RigidBody();

	glm::mat4 GetActualTransformMatrix();

	void LoadInWorld(btDiscreteDynamicsWorld* world);

private:
	btCollisionShape* m_shape;
	btDefaultMotionState* m_motionState;
	btRigidBody* m_body;

	BD_RBodyShape m_shapedef;

	EXP_Game* m_gameinstance;
};

#endif //_EXP_RIGID_BODY_H__