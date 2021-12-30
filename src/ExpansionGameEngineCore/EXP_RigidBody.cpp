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
	delete m_body;
	delete m_shp;
	delete m_motionState;
}

btRigidBody* EXP_RigidBody::GetBody() {
	return m_body;
}

vec3f EXP_RigidBody::GetWorldPosition() {
	return BTVEC_TO_VEC3F(m_body->getWorldTransform().getOrigin());
}

void EXP_RigidBody::ConstructShape() {
	m_shp = new btBoxShape(btVector3(m_scale.getX(), m_scale.getY(), m_scale.getZ()));

	btTransform trs = btTransform();
	trs.setOrigin(VEC3F_TO_BTVEC(m_pos));

	btQuaternion q = btQuaternion(m_rot.getX(), m_rot.getY(), m_rot.getZ());
	trs.setRotation(q);

	m_motionState = new btDefaultMotionState(trs);

	m_body = new btRigidBody(m_mass, m_motionState, m_shp);

	m_game->GetPhysicsHandler()->RegisterRigidBody(this);
	if (m_isKinematic) {
		m_body->setFlags(m_body->getFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	}
}

void EXP_RigidBody::AddMovementInput(vec3f direction, float scale) {
	if (!m_isKinematic) {
		m_body->applyCentralForce(VEC3F_TO_BTVEC(direction) * scale);
	}
	else {
		btTransform transf = m_body->getWorldTransform();
		btVector3 pos = transf.getOrigin();
		pos = pos + VEC3F_TO_BTVEC(direction);
		pos *= scale;

		m_body->setWorldTransform(transf);
	}
}

void EXP_RigidBody::FreezeRotationAxis(bool X, bool Y, bool Z) {
	btVector3 ang(1.0f, 1.0f, 1.0f);
	if (X)
		ang.setX(0.0f);
	if (Y)
		ang.setY(0.0f);
	if (Z)
		ang.setZ(0.0f);

	m_body->setAngularFactor(ang);
}

void EXP_RigidBody::FreezePositionAxis(bool X, bool Y, bool Z) {
	
}

vec3f EXP_RigidBody::GetLinearVelocity() {
	btVector3 vel = m_body->getLinearVelocity();

	return BTVEC_TO_VEC3F(vel);
}

void EXP_RigidBody::SetLinearVelocity(vec3f vel) {
	m_body->setLinearVelocity(VEC3F_TO_BTVEC(vel));
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
	m_shp = new btSphereShape(m_radius);

	btTransform trs = btTransform();
	trs.setOrigin(VEC3F_TO_BTVEC(m_pos));

	m_motionState = new btDefaultMotionState(trs);

	m_body = new btRigidBody(m_mass, m_motionState, m_shp);
	if (m_isKinematic) {
		m_body->setFlags(m_body->getFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	}

	m_game->GetPhysicsHandler()->RegisterRigidBody(this);
}

//RB Capsule
EXP_RB_Capsule::EXP_RB_Capsule(
	EXP_Game* game,
	vec3f pos, vec3f rot,
	float radius, float height,
	float mass, bool kinematic,
	EXP_PhysicsMaterial mat) :
	EXP_RigidBody(game, pos, rot, vec3f(), mass, kinematic, mat)
{
	m_radius = radius;
	m_height = height;

	ConstructShape();
}

void EXP_RB_Capsule::ConstructShape() {
	m_shp = new btCapsuleShape(m_radius, m_height);

	btTransform trs = btTransform();
	trs.setOrigin(VEC3F_TO_BTVEC(m_pos));

	btQuaternion q = btQuaternion(m_rot.getX(), m_rot.getY(), m_rot.getZ());
	trs.setRotation(q);

	m_motionState = new btDefaultMotionState(trs);

	m_body = new btRigidBody(m_mass, m_motionState, m_shp);
	if (m_isKinematic) {
		m_body->setCollisionFlags(m_body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	}

	m_game->GetPhysicsHandler()->RegisterRigidBody(this);
}

EXP_RB_Mesh::EXP_RB_Mesh(EXP_Game* game, vec3f pos, vec3f rot, vec3f scale, std::string meshref, float mass, EXP_PhysicsMaterial mat) :
	EXP_RigidBody(game, pos, rot, scale, mass, true, mat)
{
	BD_Reader mr = BD_Reader();
	mr.ReadMSHFile(game->GetFilePathByRef(meshref));

	for (int i = 0; i < mr.GetVerticesCount(); i++) {
		vec3f v = mr.GetVertexByIndex(i);

		m_verticies.push_back(v);
	}

	for (int i = 0; i < mr.GetIndicesCount(); i++) {
		int idc = mr.GetIndiceByIndex(i);
		m_indicies.push_back(idc);
	}

	ConstructShape();
}

void EXP_RB_Mesh::ConstructShape() {
	
	m_game->GetPhysicsHandler()->RegisterRigidBody(this);
}

EXP_CharControllerCapsule::EXP_CharControllerCapsule(EXP_Game* game, vec3f pos, vec3f rot, float height, float radius, float mass, EXP_PhysicsMaterial mat) : EXP_CharacterController() {
	m_game = game;

	m_height = height;
	m_radius = radius;

	m_pos = pos;
	m_rot = rot;
	m_mat = mat;

	ConstructController();
}

EXP_CharControllerCapsule::~EXP_CharControllerCapsule() {
	
}

void EXP_CharControllerCapsule::ConstructController() {
	m_capsule = new EXP_RB_Capsule(m_game, m_pos, m_rot, m_radius, m_height, 1.0f);
	m_capsule->FreezeRotationAxis(true, true, false);
	m_capsule->GetBody()->setRestitution(0.0f);
}

vec3f EXP_CharControllerCapsule::GetWorldPosition() {
	return m_capsule->GetWorldPosition();
}

void EXP_CharControllerCapsule::AddMovementInput(vec3f dir) {
	m_capsule->SetLinearVelocity(dir * 10.0f);
}