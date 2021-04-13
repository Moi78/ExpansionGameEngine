#include "pch.h"
#include "EXP_RigidBody.h"

#include "EXP_PhysicsHandler.h"

EXP_RigidBody::EXP_RigidBody(EXP_Game* game, vec3f pos, vec3f rot, vec3f scale, float mass, bool kinematic) : m_pos(pos), m_rot(rot), m_scale(scale){
	m_mass = mass;
	m_game = game;

	m_isKinematic = kinematic;
}

EXP_RigidBody::~EXP_RigidBody() {
	m_body->release();
}

physx::PxRigidActor* EXP_RigidBody::GetBody() {
	if (m_mass > 0) {
		return m_body;
	}
	else {
		return m_body_static;
	}
}

vec3f EXP_RigidBody::GetWorldPosition() {
	if (m_mass > 0) {
		physx::PxVec3 pos = m_body->getGlobalPose().p;
		return vec3f(pos.x, pos.y, pos.z);
	}
	else {
		physx::PxVec3 pos = m_body_static->getGlobalPose().p;
		return vec3f(pos.x, pos.y, pos.z);
	}
}

void EXP_RigidBody::ConstructShape() {
	physx::PxMaterial* mat = m_game->GetPhysicsHandler()->GetPhysics()->createMaterial(0.0f, 0.2f, 0.0f);

	physx::PxShape* shp = m_game->GetPhysicsHandler()->GetPhysics()->createShape(
		physx::PxBoxGeometry(m_scale.getX(), m_scale.getY(), m_scale.getZ()),
		*mat
	);

	mat4f transf = mat4f(1.0f);
	transf = TranslateMatrix(transf, m_pos);
	transf = RotateMatrix(transf, m_rot);

	if (m_mass > 0) {
		m_body = physx::PxCreateDynamic(
			*m_game->GetPhysicsHandler()->GetPhysics(),
			physx::PxTransform(
				physx::PxMat44(transf.GetPTR()).getTranspose()
			),
			*shp,
			1.0f
		);

		if (!m_body) {
			std::cerr << "ERROR: Could not create a PxShape (Box)." << std::endl;
			return;
		}

		m_body->setMass(m_mass);
		m_body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, m_isKinematic);
	}
	else {
		m_body_static = physx::PxCreateStatic(
			*m_game->GetPhysicsHandler()->GetPhysics(),
			physx::PxTransform(
				physx::PxMat44(transf.GetPTR()).getTranspose()
			),
			*shp
		);

		if (!m_body_static) {
			std::cerr << "ERROR: Could not create a PxShape (Box)." << std::endl;
			return;
		}
	}

	shp->release();
	mat->release();

	m_game->GetPhysicsHandler()->RegisterRigidBody(this);
}

void EXP_RigidBody::AddMovementInput(vec3f direction, float scale) {
	if (m_body) {
		//vec3f target = m_pos + direction;
		//m_body->setKinematicTarget(physx::PxTransform(target.getX() * scale, target.getY() * scale, target.getZ() * scale));
		m_body->addForce(physx::PxVec3(direction.getX() * scale, direction.getY() * scale, direction.getZ() * scale));
	}
}

void EXP_RigidBody::FreezeRotationAxis(bool X, bool Y, bool Z) {
	if (m_body) {
		m_body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, X);
		m_body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, Y);
		m_body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, Z);
	}
}

void EXP_RigidBody::FreezePositionAxis(bool X, bool Y, bool Z) {
	if (m_body) {
		m_body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, X);
		m_body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, Y);
		m_body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, Z);
	}
}

//RB Box

EXP_RB_Box::EXP_RB_Box(EXP_Game* game, vec3f pos, vec3f rot, vec3f scale, float mass, bool kinematic, vec3f inertia)  :
	EXP_RigidBody(game, pos, rot, scale, mass, kinematic) {
	ConstructShape();
}

//RB Sphere

EXP_RB_Sphere::EXP_RB_Sphere(EXP_Game* game, vec3f pos, vec3f rot, float radius, float mass, bool kinematic, vec3f inertia) :
	EXP_RigidBody(game, pos, rot, vec3f(), mass, kinematic), m_radius(radius) {
	ConstructShape();
}

void EXP_RB_Sphere::ConstructShape() {
	physx::PxMaterial* mat = m_game->GetPhysicsHandler()->GetPhysics()->createMaterial(0.0f, 0.2f, 0.0f);

	physx::PxShape* shp = m_game->GetPhysicsHandler()->GetPhysics()->createShape(
		physx::PxSphereGeometry(m_radius),
		*mat
	);

	mat4f transf = mat4f(1.0f);
	transf = TranslateMatrix(transf, m_pos);
	transf = RotateMatrix(transf, m_rot);

	if (m_mass > 0) {
		m_body = physx::PxCreateDynamic(
			*m_game->GetPhysicsHandler()->GetPhysics(),
			physx::PxTransform(
				physx::PxMat44(transf.GetPTR()).getTranspose()
			),
			*shp,
			1.0f
		);

		if (!m_body) {
			std::cerr << "ERROR: Could not create a PxShape (Sphere)." << std::endl;
			return;
		}

		m_body->setMass(m_mass);
		m_body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, m_isKinematic);
	}
	else {
		m_body_static = physx::PxCreateStatic(
			*m_game->GetPhysicsHandler()->GetPhysics(),
			physx::PxTransform(
				physx::PxMat44(transf.GetPTR()).getTranspose()
			),
			*shp
		);

		if (!m_body_static) {
			std::cerr << "ERROR: Could not create a PxShape (Sphere)." << std::endl;
			return;
		}
	}

	shp->release();
	mat->release();

	m_game->GetPhysicsHandler()->RegisterRigidBody(this);
}

//RB Capsule
EXP_RB_Capsule::EXP_RB_Capsule(EXP_Game* game, vec3f pos, vec3f rot, float radius, float height, float mass, bool kinematic, vec3f inertia) :
	EXP_RigidBody(game, pos, rot, vec3f(), mass, kinematic)
{
	m_radius = radius;
	m_height = height;

	ConstructShape();
}

void EXP_RB_Capsule::ConstructShape() {
	physx::PxMaterial* mat = m_game->GetPhysicsHandler()->GetPhysics()->createMaterial(0.0f, 0.0f, 0.0f);

	physx::PxShape* shp = m_game->GetPhysicsHandler()->GetPhysics()->createShape(
		physx::PxCapsuleGeometry(m_radius, m_height),
		*mat
	);

	mat4f transf = mat4f(1.0f);
	transf = TranslateMatrix(transf, m_pos);
	transf = RotateMatrix(transf, m_rot);

	if (m_mass > 0) {
		m_body = physx::PxCreateDynamic(
			*m_game->GetPhysicsHandler()->GetPhysics(),
			physx::PxTransform(
				physx::PxMat44(transf.GetPTR()).getTranspose()
			),
			*shp,
			1.0f
		);

		if (!m_body) {
			std::cerr << "ERROR: Could not create a PxShape (Capsule)." << std::endl;
			return;
		}

		m_body->setMass(m_mass);
		m_body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, m_isKinematic);
	}
	else {
		m_body_static = physx::PxCreateStatic(
			*m_game->GetPhysicsHandler()->GetPhysics(),
			physx::PxTransform(
				physx::PxMat44(transf.GetPTR()).getTranspose()
			),
			*shp
		);

		if (!m_body_static) {
			std::cerr << "ERROR: Could not create a PxShape (Capsule)." << std::endl;
			return;
		}
	}

	shp->release();
	mat->release();

	m_game->GetPhysicsHandler()->RegisterRigidBody(this);
}