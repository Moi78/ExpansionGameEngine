#include "pch.h"
#include "RD_Materials.h"

RD_ShaderMaterial::RD_ShaderMaterial(RD_ShaderLoader* shader) {
	m_shader = shader;
}

RD_ShaderMaterial::~RD_ShaderMaterial() {
	if(m_shader)
		delete m_shader;
}

void RD_ShaderMaterial::BindMaterial() {
	//Textures uniforms
	unsigned int TexID = GL_TEXTURE10;
	for (auto m : m_textures) {
		glActiveTexture(TexID);
		glBindTexture(GL_TEXTURE_2D, m.second);
		m_shader->SetInt(m.first, TexID - GL_TEXTURE0);

		TexID++;
	}
}

RD_ShaderLoader* RD_ShaderMaterial::GetShader() {
	return m_shader;
}

void RD_ShaderMaterial::AddTexture(std::string paramName, unsigned int id) {
	m_textures.push_back(std::pair<std::string, unsigned int>(paramName, id));
}