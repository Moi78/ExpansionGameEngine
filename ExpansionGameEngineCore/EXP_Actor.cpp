#include "pch.h"
#include "EXP_Actor.h"

#include "EXP_Game.h"

EXP_Actor::EXP_Actor(EXP_Game* game, vec3f pos, vec3f rot, vec3f scale, std::function<void()> tick, std::function<void()> onStart, std::function<void()> OnUnregister) {
	m_game = game;

	m_pos = pos;
	m_rot = rot;
	m_scale = scale;

	m_onStart = new RD_Callback(onStart);
	m_tick = new RD_Callback(tick);
	m_onUnregister = new RD_Callback(OnUnregister);

	m_game->RegisterActor(this);
}

EXP_Actor::~EXP_Actor() {
	delete m_onStart;
	delete m_tick;
	delete m_onUnregister;
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