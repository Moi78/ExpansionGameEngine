#include "pch.h"
#include "EXP_RigidBody.h"

#include "EXP_PhysicsHandler.h"

EXP_RigidBody::EXP_RigidBody(EXP_Game* game, vec3f pos, vec3f rot, vec3f scale, float mass, bool kinematic, EXP_PhysicsMaterial mat) :
	m_pos(pos), m_rot(rot), m_scale(scale)
{
	m_mat = mat;

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
	physx::PxMaterial* mat = m_game->GetPhysicsHandler()->GetPhysics()->createMaterial(
		m_mat.StaticFriction, m_mat.DynamicFriction, m_mat.Restitution
	);

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

vec3f EXP_RigidBody::GetLinearVelocity() {
	if (m_body) {
		physx::PxVec3 v = m_body->getLinearVelocity();
		return vec3f(v.x, v.y, v.z);
	}

	return vec3f();
}

//RB Box

EXP_RB_Box::EXP_RB_Box(EXP_Game* game, vec3f pos, vec3f rot, vec3f scale, float mass, bool kinematic, EXP_PhysicsMaterial mat)  :
	EXP_RigidBody(game, pos, rot, scale, mass, kinematic, mat) {
	ConstructShape();
}

//RB Sphere

EXP_RB_Sphere::EXP_RB_Sphere(EXP_Game* game, vec3f pos, vec3f rot, float radius, float mass, bool kinematic, EXP_PhysicsMaterial mat) :
	EXP_RigidBody(game, pos, rot, vec3f(), mass, kinematic, mat), m_radius(radius) {
	ConstructShape();
}

void EXP_RB_Sphere::ConstructShape() {
	physx::PxMaterial* mat = m_game->GetPhysicsHandler()->GetPhysics()->createMaterial(
		m_mat.StaticFriction, m_mat.DynamicFriction, m_mat.Restitution
	);

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
EXP_RB_Capsule::EXP_RB_Capsule(EXP_Game* game, vec3f pos, vec3f rot, float radius, float height, float mass, bool kinematic, EXP_PhysicsMaterial mat) :
	EXP_RigidBody(game, pos, rot, vec3f(), mass, kinematic, mat)
{
	m_radius = radius;
	m_height = height;

	ConstructShape();
}

void EXP_RB_Capsule::ConstructShape() {
	physx::PxMaterial* mat = m_game->GetPhysicsHandler()->GetPhysics()->createMaterial(
		m_mat.StaticFriction, m_mat.DynamicFriction, m_mat.Restitution
	);

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

EXP_RB_Mesh::EXP_RB_Mesh(EXP_Game* game, vec3f pos, vec3f rot, vec3f scale, std::string meshref, float mass, EXP_PhysicsMaterial mat) : 
	EXP_RigidBody(game, pos, rot, scale, mass, true, mat)
{
	BD_Reader mr = BD_Reader();
	mr.ReadMSHFile(game->GetFilePathByRef(meshref));

	for (int i = 0; i < mr.GetVerticesCount(); i++) {
		vec3f v = mr.GetVertexByIndex(i);

		m_verticies.push_back(physx::PxVec3(v.getX(), v.getY(), v.getZ()));
	}

	for (int i = 0; i < mr.GetIndicesCount(); i++) {
		int idc = mr.GetIndiceByIndex(i);
		m_indicies.push_back(idc);
	}

	ConstructShape();
}

void EXP_RB_Mesh::ConstructShape() {
	using namespace physx;

	PxTriangleMeshDesc mDesc;

	mDesc.points.count = m_verticies.size();
	mDesc.points.stride = sizeof(PxVec3);
	mDesc.points.data = m_verticies.data();

	mDesc.triangles.count = m_indicies.size();
	mDesc.triangles.stride = 3 * sizeof(int32_t);
	mDesc.triangles.data = m_indicies.data();

	PxDefaultMemoryOutputStream buf;
	PxCooking* cooker = m_game->GetPhysicsHandler()->GetCooker();
	if (!cooker->cookTriangleMesh(mDesc, buf)) {
		std::cerr << "ERROR: Could not cook mesh data." << std::endl;
		return;
	}

	PxDefaultMemoryInputData rbuf(buf.getData(), buf.getSize());
	PxTriangleMesh* mesh_data = m_game->GetPhysicsHandler()->GetPhysics()->createTriangleMesh(rbuf);

	mat4f transf = mat4f(1.0f);
	transf = TranslateMatrix(transf, m_pos);
	transf = RotateMatrix(transf, m_rot);

	PxPhysics* phx = m_game->GetPhysicsHandler()->GetPhysics();

	m_body = phx->createRigidDynamic(PxTransform(physx::PxMat44(transf.GetPTR()).getTranspose()));

	if (!m_body) {
		std::cerr << "ERROR: Could not create a rigid body (Mesh)." << std::endl;
		return;
	}

	m_body->setMass(m_mass);
	m_body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);

	PxTriangleMeshGeometry triGeom;
	triGeom.triangleMesh = mesh_data;

	PxMaterial* mat = phx->createMaterial(m_mat.StaticFriction, m_mat.DynamicFriction, m_mat.Restitution);
	PxShape* shp = PxRigidActorExt::createExclusiveShape(*m_body, triGeom, *mat);

	mat->release();

	m_game->GetPhysicsHandler()->RegisterRigidBody(this);
}