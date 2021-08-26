#include "pch.h"
#include "EXP_StaticMesh.h"

EXP_StaticMesh::EXP_StaticMesh(EXP_Game* gameinstance, RD_ShaderMaterial* shader, std::string MeshRef, vec3f pos, vec3f rot, vec3f scale, bool useSystemTree) :
	EXP_Component(pos, rot, scale), m_gameinstance(gameinstance),
	RD_Mesh(gameinstance->GetRenderer(), shader, pos, rot, scale)
{
	m_useSystemTree = useSystemTree;
	LoadMesh(MeshRef);
}

EXP_StaticMesh::~EXP_StaticMesh() {

}

bool EXP_StaticMesh::MeshRefExists(std::string MeshRef) {
	if (!m_useSystemTree) {
		return std::filesystem::exists(std::filesystem::path(m_gameinstance->GetGameInfo().RootGameContentFolder + MeshRef));
	}
	else {
		return std::filesystem::exists(MeshRef);
	}
}

void EXP_StaticMesh::LoadMesh(std::string MeshRef) {
	if (MeshRefExists(MeshRef)) {
		if (!m_useSystemTree) {
			RD_Mesh::loadMesh(m_gameinstance->GetGameInfo().RootGameContentFolder + MeshRef);
		}
		else {
			RD_Mesh::loadMesh(MeshRef);
		}

		m_gameinstance->RegisterMesh(this);
	}
	else {
		std::cerr << "ERROR (EXP_StaticMesh::LoadMesh): Can't load mesh " << MeshRef << ". Mesh Reference not found." << std::endl;
		dispErrorMessageBox(StrToWStr("Can't load mesh " + MeshRef + ". Mesh reference not found."));
	}

	m_meshref = MeshRef;
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

void EXP_StaticMesh::UseParentMatrix(mat4f parent) {
	RD_Mesh::SetParentMatrix(parent);
}

std::string EXP_StaticMesh::GetMeshRef() {
	return m_meshref;
}

void EXP_StaticMesh::SetMetaInf(const std::string& meta) {
	m_meta = meta;
}

std::string EXP_StaticMesh::GetMetaInf() {
	return m_meta;
}