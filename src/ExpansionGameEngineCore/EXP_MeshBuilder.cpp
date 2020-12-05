#include "pch.h"
#include "EXP_MeshBuilder.h"

EXP_MeshBuilder::EXP_MeshBuilder(EXP_Game* game, RD_ShaderMaterial* shader, vec3f pos, vec3f rot, vec3f scale) :
	RD_MeshBuilder(game->GetRenderer(), shader, pos, rot, scale), EXP_Component(pos, rot, scale) {

	game->RegisterMesh(GetMesh());
}

EXP_MeshBuilder::~EXP_MeshBuilder() {

}

void EXP_MeshBuilder::SetPosition(vec3f nPos) {
	EXP_Component::m_pos = nPos;
	GetMesh()->SetPosition(nPos);
}

void EXP_MeshBuilder::SetRotation(vec3f nRot) {
	EXP_Component::m_rot = nRot;
	GetMesh()->SetRotation(nRot);
}

void EXP_MeshBuilder::SetScale(vec3f nScale) {
	EXP_Component::m_scale = nScale;
	GetMesh()->SetScale(nScale);
}

void EXP_MeshBuilder::UseParentMatrix(mat4f parent) {
	GetMesh()->SetParentMatrix(parent);
}