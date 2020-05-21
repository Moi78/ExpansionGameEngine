#include "pch.h"
#include "EXP_Component.h"

EXP_Component::EXP_Component(vec3f pos, vec3f rot, vec3f scale) : m_worldPos(pos), m_worldRot(rot), m_worldScale(scale) {

}

EXP_Component::~EXP_Component() {

}

vec3f EXP_Component::GetPosition() {
	return m_pos;
}

vec3f EXP_Component::GetRotation() {
	return m_rot;
}

vec3f EXP_Component::GetScale() {
	return m_scale;
}

void EXP_Component::SetPosition(vec3f nPos) {
	m_pos = nPos;
}

void EXP_Component::SetRotation(vec3f nRot) {
	m_rot = nRot;
}

void EXP_Component::SetScale(vec3f nScale) {
	m_scale = nScale;
}

void EXP_Component::SetNameTag(std::string nTagName) {
	m_nameTag = nTagName;
}

std::string EXP_Component::GetNameTag() {
	return m_nameTag;
}

vec3f EXP_Component::GetWorldPosition() {
	return m_worldPos;
}

vec3f EXP_Component::GetWorldRotation() {
	return m_worldRot;
}

vec3f EXP_Component::GetWorldScale() {
	return m_worldScale;
}

void EXP_Component::SetWorldPosition(vec3f nPos) {
	m_worldPos = nPos;
}

void EXP_Component::SetWorldRotation(vec3f nRot) {
	m_worldRot = nRot;
}

void EXP_Component::SetWorldScale(vec3f nScale) {
	m_worldScale = nScale;
}