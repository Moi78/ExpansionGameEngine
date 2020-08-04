#include "pch.h"
#include "EXP_Actor.h"

#include "EXP_Game.h"

EXP_Actor::EXP_Actor(EXP_Game* game, vec3f pos, vec3f rot, vec3f scale) {
	m_game = game;

	m_pos = pos;
	m_rot = rot;
	m_scale = scale;

	UpdateActorMatrix();

	m_game->RegisterActor(this);
}

EXP_Actor::~EXP_Actor() {

}

vec3f EXP_Actor::GetWorldPos() {
	return m_pos;
}

vec3f EXP_Actor::GetWorldRot() {
	return m_rot;
}

vec3f EXP_Actor::GetWorldScale() {
	return m_scale;
}

void EXP_Actor::SetWorldPos(vec3f nPos) {
	m_pos = nPos;
	UpdateActorMatrix();
	UpdateActor();
}

void EXP_Actor::SetWorldRot(vec3f nRot) {
	m_rot = nRot;
	UpdateActorMatrix();
	UpdateActor();
}

void EXP_Actor::SetWorldScale(vec3f nScale) {
	m_scale = nScale;
	UpdateActorMatrix();
	UpdateActor();
}

void EXP_Actor::AddWorldPos(vec3f apos) {
	m_pos = m_pos + apos;
	UpdateActorMatrix();
	UpdateActor();
}

void EXP_Actor::AddWorldRot(vec3f arot) {
	m_rot = m_rot + arot;
	UpdateActorMatrix();
	UpdateActor();
}

void EXP_Actor::AddWorldScale(vec3f ascale) {
	m_scale = m_scale + ascale;
	UpdateActorMatrix();
	UpdateActor();
}

void EXP_Actor::Start() {

}

void EXP_Actor::Tick() {

}

void EXP_Actor::Unregister() {

}

glm::mat4 EXP_Actor::GenerateActorMatrix() {
	glm::mat4 mat = glm::mat4(1.0f);

	mat = glm::translate(mat, glm::vec3(m_pos.getX(), m_pos.getY(), m_pos.getZ()));
	mat = glm::scale(mat, glm::vec3(m_scale.getX(), m_scale.getY(), m_scale.getZ()));

	mat = glm::rotate(mat, glm::radians(m_rot.getX()), glm::vec3(1.0f, 0.0, 0.0));
	mat = glm::rotate(mat, glm::radians(m_rot.getY()), glm::vec3(0.0f, 1.0, 0.0));
	mat = glm::rotate(mat, glm::radians(m_rot.getZ()), glm::vec3(0.0f, 0.0, 1.0));

	return mat;
}

void EXP_Actor::UpdateActor() {
	for (auto c : m_comps) {
		c->UseParentMatrix(m_actor_mat);
	}
}

void EXP_Actor::UpdateActorMatrix() {
	m_actor_mat = GenerateActorMatrix();
}

void EXP_Actor::LinkComponent(EXP_Component* comp) {
	m_comps.push_back(comp);
	comp->UseParentMatrix(m_actor_mat);
}