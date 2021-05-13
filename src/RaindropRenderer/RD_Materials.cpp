#include "pch.h"
#include "RD_Materials.h"
#include "RD_Mesh.h"

RD_ShaderMaterial::RD_ShaderMaterial(RD_ShaderLoader* shader) {
	m_shader = shader;
}

RD_ShaderMaterial::~RD_ShaderMaterial() {
	std::cout << "Deleting shader material" << std::endl;

	delete m_shader;

	for (auto tex : m_textures) {
		delete tex.second;
	}
	m_textures.clear();
}

void RD_ShaderMaterial::BindMaterial() {
	//Textures uniforms
	unsigned int TexID = 10;
	for (auto m : m_textures) {
		m.second->BindTexture(TexID);
		m_shader->SetInt(m.first, TexID);

		TexID++;
	}
}

RD_ShaderLoader* RD_ShaderMaterial::GetShader() {
	return m_shader;
}

void RD_ShaderMaterial::AddTexture(std::string paramName, RD_Texture* tex) {
	m_textures.push_back(std::pair<std::string, RD_Texture*>(paramName, tex));
}

void RD_ShaderMaterial::RegisterMeshReference(RD_Mesh* msh) {
	m_meshes_references.push_back(msh);
}

void RD_ShaderMaterial::UnregisterMeshReference(RD_Mesh* msh) {
	const int index = GetElemIndex<RD_Mesh*>(m_meshes_references, msh);

	if (index != -1) {
		m_meshes_references.erase(m_meshes_references.begin() + index);
	}
	else {
		std::cerr << "ERROR: Element does not exists" << std::endl;
	}
}

void RD_ShaderMaterial::DrawMeshes() {
	m_shader->useShader();
	BindMaterial();

	for (auto m : m_meshes_references) {
		m->render();
	}
}

void RD_ShaderMaterial::SetShader(RD_ShaderLoader* shader, bool nodelete) {
	if (!nodelete) {
		delete m_shader;
	}

	m_shader = shader;
}

void RD_ShaderMaterial::PurgeTextures() {
	for (auto tex : m_textures) {
		delete tex.second;
	}
	m_textures.clear();
}