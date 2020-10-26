#include "pch.h"
#include "EXP_InstanciatedMesh.h"

EXP_InstanciatedMesh::EXP_InstanciatedMesh(EXP_Game* game, RD_ShaderMaterial* shader, std::string meshRef) : EXP_Component(vec3f(), vec3f(), vec3f()), RD_InstanciatedMesh(shader), m_game(game) {
	LoadMesh(meshRef);
}

EXP_InstanciatedMesh::~EXP_InstanciatedMesh() {

}

bool EXP_InstanciatedMesh::MeshRefExists(std::string ref) {
	std::string RealRef = m_game->GetGameInfo().RootGameContentFolder + ref + ".msh";
	return std::filesystem::exists(std::filesystem::path(RealRef));
}

void EXP_InstanciatedMesh::LoadMesh(std::string meshRef) {
	if (MeshRefExists(meshRef)) {
		RD_InstanciatedMesh::loadMesh(m_game->GetGameInfo().RootGameContentFolder + meshRef + ".msh");

		m_game->RegisterMesh(this);
	}
	else {
		std::cerr << "ERROR: Can't load mesh " << meshRef << ". Mesh Reference not found." << std::endl;
		dispErrorMessageBox(StrToWStr("Can't load mesh " + meshRef + ". Mesh reference not found."));
	}
}

void EXP_InstanciatedMesh::UseParentMatrix(mat4f parent) {
	RD_InstanciatedMesh::SetParentMatrix(parent);
}