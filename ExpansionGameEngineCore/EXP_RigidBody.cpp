#include "pch.h"
#include "EXP_RigidBody.h"


EXP_RigidBody::EXP_RigidBody(EXP_Game* game, BD_RBodyShape shape, float mass) {
	m_shapedef = shape;
	m_gameinstance = game;

	if (shape.type == BD_RBodyShapeType::CUBE) {
		m_shape = new btBoxShape(btVector3(shape.scale.getX(), shape.scale.getY(), shape.scale.getZ()));

	}
	else if (shape.type == BD_RBodyShapeType::CYLINDER) {
		m_shape = new btCylinderShape(btVector3(1.0f, 1.0f, 1.0f));

	}
	else if (shape.type == BD_RBodyShapeType::CAPSULE) {
		m_shape = new btCapsuleShape(shape.radius, shape.height);
	}

	if (!m_shape) {
		std::cerr << "Error while creating new Bullet Shape." << std::endl;
		return;
	}

	btTransform shapetrans;
	shapetrans.setIdentity();
	shapetrans.setOrigin(btVector3(shape.pos.getX(), shape.pos.getY(), shape.pos.getZ()));
	shapetrans.setRotation(btQuaternion(shape.rot.getX(), shape.rot.getY(), shape.rot.getZ(), 0.0f));

	if (mass != 0.0f) {
		btVector3 inertia(0.0f, 0.0f, 0.0f);

		m_shape->calculateLocalInertia(mass, inertia);
	}

	m_motionState = new btDefaultMotionState(shapetrans);

	btRigidBody::btRigidBodyConstructionInfo constructionInfos(mass, m_motionState, m_shape);
	m_body = new btRigidBody(constructionInfos);

	if (!m_body) {
		std::cerr << "Error while creating RigidBody. " << std::endl;
	}

	m_gameinstance->RegisterRigidBody(this);
}

EXP_RigidBody::~EXP_RigidBody() {

}

glm::mat4 EXP_RigidBody::GetActualTransformMatrix() {
	btTransform t;
	m_body->getMotionState()->getWorldTransform(t);

	btScalar mat[16];
	t.getOpenGLMatrix(mat);

	for (int i = 0; i < 16; i++) {
		std::cout << mat[i] << std::endl;
	}

	return glm::mat4(1.0f);
}

void EXP_RigidBody::LoadInWorld(btDiscreteDynamicsWorld* world) {
	world->addRigidBody(m_body);
}