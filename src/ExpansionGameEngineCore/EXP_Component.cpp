#include "pch.h"
#include "EXP_Component.h"

EXP_Component::EXP_Component(vec3f pos, vec3f rot, vec3f scale) : m_pos(pos), m_rot(rot), m_scale(scale), m_parent_mat(1.0f), m_nameTag("") {

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

void EXP_Component::UseParentMatrix(glm::mat4 mat) {
	m_parent_mat = mat;
}

glm::mat4 EXP_Component::GetParentMatrix() {
	return m_parent_mat;
}