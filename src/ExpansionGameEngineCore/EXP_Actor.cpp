#include "pch.h"
#include "EXP_Actor.h"

#include "EXP_Game.h"

EXP_Actor::EXP_Actor(EXP_Game* game, vec3f pos, vec3f rot, vec3f scale, std::function<void()> tick, std::function<void()> onStart, std::function<void()> OnUnregister) {
	m_game = game;

	m_pos = pos;
	m_rot = rot;
	m_scale = scale;

	m_onStart = std::make_unique<RD_Callback>(onStart);
	m_tick = std::make_unique<RD_Callback>(tick);
	m_onUnregister = std::make_unique<RD_Callback>(OnUnregister);

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

void EXP_Actor::CallTick() {
	m_tick->Call();
}

void EXP_Actor::CallOnStart() {
	m_onStart->Call();
}

void EXP_Actor::CallUnregister() {
	m_onUnregister->Call();
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
	CallTick();
}