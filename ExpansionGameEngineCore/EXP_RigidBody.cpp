#include "pch.h"
#include "EXP_RigidBody.h"

#include "EXP_PhysicsHandler.h"

EXP_RigidBody::EXP_RigidBody(EXP_Game* game, vec3f pos, vec3f rot, vec3f scale, float mass) : m_pos(pos), m_rot(rot), m_scale(scale){
	m_mass = mass;
	m_game = game;

	ConstructShape();
}

EXP_RigidBody::~EXP_RigidBody() {

}

btRigidBody* EXP_RigidBody::GetBody() {
	return m_body;
}

vec3f EXP_RigidBody::GetWorldPosition() {
	btTransform trans;

	m_body->getMotionState()->getWorldTransform(trans);

	return vec3f(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
}

void EXP_RigidBody::ConstructShape() {
	m_shape = new btBoxShape(btVector3(m_scale.getX(), m_scale.getY(), m_scale.getZ()));

	btTransform shapePosRot;
	shapePosRot.setIdentity();
	shapePosRot.setOrigin(btVector3(m_pos.getX(), m_pos.getY(), m_pos.getZ()));

	btQuaternion quat;
	quat.setEuler(m_rot.getX(), m_rot.getY(), m_rot.getZ());
	shapePosRot.setRotation(quat);

	btVector3 localInertia(0, 0, 0);

	if (m_mass > 0) {
		m_shape->calculateLocalInertia(m_mass, localInertia);
	}

	m_motionState = new btDefaultMotionState(shapePosRot);

	btRigidBody::btRigidBodyConstructionInfo constructInfo(m_mass, m_motionState, m_shape, localInertia);

	m_body = new btRigidBody(constructInfo);

	m_game->GetPhysicsHandler()->RegisterRigidBody(this);
}

//RB Box

EXP_RB_Box::EXP_RB_Box(EXP_Game* game, vec3f pos, vec3f rot, vec3f scale, float mass, vec3f inertia) : EXP_RigidBody(game, pos, rot, scale, mass) {
	ConstructShape();
}

//RB Sphere

EXP_RB_Sphere::EXP_RB_Sphere(EXP_Game* game, vec3f pos, vec3f rot, float radius, float mass, vec3f inertia) : EXP_RigidBody(game, pos, rot, vec3f(), mass), m_radius(radius) {
	ConstructShape();
}

void EXP_RB_Sphere::ConstructShape() {
	m_shape = new btSphereShape(m_radius);

	btTransform transPosRot;
	transPosRot.setIdentity();
	transPosRot.setOrigin(btVector3(m_pos.getX(), m_pos.getY(), m_pos.getZ()));

	btQuaternion quat;
	quat.setEuler(m_rot.getX(), m_rot.getY(), m_rot.getZ());
	transPosRot.setRotation(quat);

	btVector3 localInertia(0, 0, 0);

	if (m_mass > 0) {
		m_shape->calculateLocalInertia(m_mass, localInertia);
	}

	m_motionState = new btDefaultMotionState(transPosRot);

	btRigidBody::btRigidBodyConstructionInfo constructInfo(m_mass, m_motionState, m_shape, localInertia);

	m_body = new btRigidBody(constructInfo);

	m_game->GetPhysicsHandler()->RegisterRigidBody(this);
}