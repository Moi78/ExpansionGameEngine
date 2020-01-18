#include "pch.h"
#include "EXP_Actor.h"

EXP_Actor::EXP_Actor(vec3f pos, vec3f rot, vec3f scale) {
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