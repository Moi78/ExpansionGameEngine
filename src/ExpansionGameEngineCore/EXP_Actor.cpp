#include "pch.h"
#include "EXP_Actor.h"

#include "EXP_Game.h"

EXP_Actor::EXP_Actor(EXP_Game* game, vec3f pos, vec3f rot, vec3f scale) : m_actor_mat(1.0f) {
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

mat4f EXP_Actor::GenerateActorMatrix() {
	mat4f trans = mat4f(1.0f);
	mat4f scale = mat4f(1.0f);
	mat4f rot = mat4f(1.0f);

	trans = TranslateMatrix(trans, m_pos);
	scale = ScaleMatrix(scale, m_scale);
	rot = RotateMatrix(rot, m_rot);

	return trans * scale * rot;
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