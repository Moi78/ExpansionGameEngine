#pragma once

#ifndef EXP_RIGID_BODY_H__
#define EXP_RIGID_BODY_H__

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

#include "EXP_Game.h"

#include <PxPhysicsAPI.h>

//Forward Declaration
class EXP_PhysicsHandler;

struct EXPGE_API EXP_PhysicsMaterial {
	float StaticFriction = 0.0f;
	float DynamicFriction = 0.1f;
	float Restitution = 0.0f;
};

class EXPGE_API EXP_RigidBody
{
public:
	EXP_RigidBody(EXP_Game* game, vec3f pos, vec3f rot, vec3f scale, float mass, bool kinematic = false, EXP_PhysicsMaterial mat = {});
	virtual ~EXP_RigidBody();

	physx::PxRigidActor* GetBody();
	vec3f GetWorldPosition();

	void AddMovementInput(vec3f direction, float scale);
	void FreezeRotationAxis(bool X, bool Y, bool Z);
	void FreezePositionAxis(bool X, bool Y, bool Z);
	vec3f GetLinearVelocity();

	virtual void ConstructShape();

protected:
	vec3f m_pos, m_rot, m_scale;
	physx::PxRigidDynamic* m_body;
	physx::PxRigidStatic* m_body_static; //UGLYYYYYYY

	EXP_PhysicsMaterial m_mat;

	float m_mass;
	bool m_isKinematic;

	EXP_Game* m_game;
};

class EXPGE_API EXP_RB_Box : public EXP_RigidBody {
public:
	EXP_RB_Box(EXP_Game* game, vec3f pos, vec3f rot, vec3f scale, float mass, bool kinematic = false, vec3f inertia = vec3f());
};

class EXPGE_API EXP_RB_Sphere : public EXP_RigidBody {
public:
	EXP_RB_Sphere(EXP_Game* game, vec3f pos, vec3f rot, float radius, float mass, bool kinematic = false, vec3f inertia = vec3f());

	virtual void ConstructShape() override;

private:
	float m_radius;
};

class EXPGE_API EXP_RB_Capsule : public EXP_RigidBody {
public:
	EXP_RB_Capsule(EXP_Game* game, vec3f pos, vec3f rot, float radius, float height, float mass, bool kinematic = false, vec3f inertia = vec3f());

	virtual void ConstructShape() override;

private:
	float m_radius;
	float m_height;
};

#endif //EXP_RIGID_BODY_H__