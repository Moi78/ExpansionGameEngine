#include "pch.h"
#include "RD_Materials.h"

//RD_SimpleMaterial

RD_SimpleMaterial::RD_SimpleMaterial(BD_MatDef mdef) {
	m_BaseColor = mdef.BaseColor;

	m_Specular = mdef.Specular;
	m_Shininess = mdef.Shininess;
}

RD_SimpleMaterial::~RD_SimpleMaterial() {
	glDeleteTextures(1, &m_BaseColor);
	glDeleteTextures(1, &m_Specular);
}

void RD_SimpleMaterial::BindMaterial(RD_ShaderLoader* shader) {
	UpdateColor(shader);
	UpdateSpecular(shader);
}

void RD_SimpleMaterial::UpdateColor(RD_ShaderLoader* shader) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_BaseColor);

	shader->SetInt("BaseColor", 0);
}

void RD_SimpleMaterial::UpdateSpecular(RD_ShaderLoader* shader) {
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_Specular);

	shader->SetInt("Specular", 2);

	shader->SetFloat("specularExp", m_Shininess);
}