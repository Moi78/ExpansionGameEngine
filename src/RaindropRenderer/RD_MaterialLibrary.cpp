#include "pch.h"
#include "RD_MaterialLibrary.h"

RD_MaterialLibrary::RD_MaterialLibrary() {}

RD_MaterialLibrary::~RD_MaterialLibrary() {}

void RD_MaterialLibrary::AddMaterialToLib(RD_ShaderMaterial* mat, std::string matName) {
	std::pair<std::string, RD_ShaderMaterial*> matPair(matName, mat); //Making a pair of a material and its names

	m_materials.push_back(matPair);
}

void RD_MaterialLibrary::RemoveMaterialFromLib(std::string matName) {
	//Searching material over registered one
	for (int i = 0; i < m_materials.size(); i++) {
		if (m_materials[i].first == matName) {
			m_materials.erase(m_materials.begin() + i); //Delete material
			return;
		}
	}

	std::cerr << "Material " << matName << " was not found. Nothing was deleted." << std::endl;
}

bool RD_MaterialLibrary::DoMaterialExists(std::string name) {
	//Comparing each material names with provided name
	for (auto mat : m_materials) {
		if (mat.first == name) {
			return true;
		}
	}

	return false;
}

RD_ShaderMaterial* RD_MaterialLibrary::GetMaterialByName(std::string name) {
	//Same logic as DoMaterialExists()
	for (auto mat : m_materials) {
		if (mat.first == name) {
			return mat.second;
		}
	}

	std::cerr << "Material " << name << " was not found. Returning void struct." << std::endl;
	return {};
}

void RD_MaterialLibrary::ClearLibrary() {
	for (auto mat : m_materials) {
		delete mat.second;
	}
	m_materials.clear();
}