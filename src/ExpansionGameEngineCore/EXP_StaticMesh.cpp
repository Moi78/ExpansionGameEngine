#include "pch.h"
#include "EXP_StaticMesh.h"

EXP_StaticMesh::EXP_StaticMesh(EXP_Game* gameinstance, RD_ShaderMaterial* shader, std::string MeshRef, vec3f pos, vec3f rot, vec3f scale) :
	EXP_Component(pos, rot, scale), m_gameinstance(gameinstance),
	RD_Mesh(shader, pos, rot, scale)
{
	LoadMesh(MeshRef);
}

EXP_StaticMesh::~EXP_StaticMesh() {

}

bool EXP_StaticMesh::MeshRefExists(std::string MeshRef) {
	return std::filesystem::exists(std::filesystem::path(m_gameinstance->GetGameInfo().RootGameContentFolder + MeshRef + ".msh"));
}

void EXP_StaticMesh::LoadMesh(std::string MeshRef) {
	if (MeshRefExists(MeshRef)) {
		RD_Mesh::loadMesh(m_gameinstance->GetGameInfo().RootGameContentFolder + MeshRef + ".msh");

		m_gameinstance->RegisterMesh(this);
	}
	else {
		std::cerr << "Can't load mesh " << MeshRef << ". Mesh Reference not found." << std::endl;
		dispErrorMessageBox(StrToWStr("Can't load mesh " + MeshRef + ". Mesh reference not found."));
	}
}

void EXP_StaticMesh::SetPosition(vec3f nPos) {
	EXP_Component::m_pos = nPos;
	RD_Mesh::SetPosition(nPos);
}

void EXP_StaticMesh::SetRotation(vec3f nRot) {
	EXP_Component::m_rot = nRot;
	RD_Mesh::SetRotation(nRot);
}

void EXP_StaticMesh::SetScale(vec3f nScale) {
	EXP_Component::m_scale = nScale;
	RD_Mesh::SetScale(nScale);
}

void EXP_StaticMesh::SetWorldPosition(vec3f nPos) {
	EXP_Component::m_worldPos = nPos;
	RD_Mesh::SetWorldPosition(nPos);
}

void EXP_StaticMesh::SetWorldRotation(vec3f nRot) {
	EXP_Component::m_worldRot = nRot;
	RD_Mesh::SetWorldRotation(nRot);
}

void EXP_StaticMesh::SetWorldScale(vec3f nScale) {
	EXP_Component::m_worldScale = nScale;
	RD_Mesh::SetWorldScale(nScale);
}