#include "pch.h"
#include "EXP_StaticMesh.h"

EXP_StaticMesh::EXP_StaticMesh(EXP_Game* gameinstance, std::string MeshRef, BD_MatDef material, vec3f pos, vec3f rot, vec3f scale) : EXP_Component(pos, rot, scale), m_gameinstance(gameinstance), m_material(material) {
	LoadMesh(MeshRef);
}

EXP_StaticMesh::~EXP_StaticMesh() {
	delete m_RawMesh;
}

RD_Mesh* EXP_StaticMesh::GetRawMeshData() {
	return m_RawMesh;
}

bool EXP_StaticMesh::MeshRefExists(std::string MeshRef) {
	return std::filesystem::exists(std::filesystem::path(m_gameinstance->GetGameInfo().RootGameContentFolder + MeshRef));
}

void EXP_StaticMesh::LoadMesh(std::string MeshRef) {
	if (MeshRefExists(MeshRef)) {
		m_RawMesh = new RD_Mesh(m_gameinstance->GetRenderer()->GetShader(), m_material, GetPosition(), GetRotation(), GetScale());
		m_RawMesh->loadMesh(m_gameinstance->GetGameInfo().RootGameContentFolder + MeshRef);
	}
	else {
		std::cerr << "Can't load mesh " << MeshRef << ". Mesh Reference not found." << std::endl;
	}
}