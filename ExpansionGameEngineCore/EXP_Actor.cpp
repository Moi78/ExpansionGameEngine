#include "pch.h"
#include "EXP_Actor.h"

#include "EXP_Game.h"

EXP_Actor::EXP_Actor(EXP_Game* game, vec3f pos, vec3f rot, vec3f scale, std::string actordef_path) {
	m_game = game;

	m_pos = pos;
	m_rot = rot;
	m_scale = scale;

	
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