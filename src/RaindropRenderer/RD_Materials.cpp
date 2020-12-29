#include "pch.h"
#include "RD_Materials.h"

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