#include "pch.h"
#include "RD_Materials.h"

//RD_SimpleMaterial

RD_SimpleMaterial::RD_SimpleMaterial(RD_ShaderLoader* shader, BD_MatDef mdef) {
	m_shader = shader;

	m_BaseColor = mdef.BaseColor;

	m_Specular = mdef.Specular;
	m_Shininess = mdef.Shininess;
}

RD_SimpleMaterial::~RD_SimpleMaterial() {
	delete m_shader;
}

void RD_SimpleMaterial::SetBaseColor(unsigned int nColor) {
	m_BaseColor = nColor;

	UpdateColor();
}

void RD_SimpleMaterial::BindMaterial() {
	UpdateColor();
	UpdateSpecular();
}

void RD_SimpleMaterial::UpdateColor() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_BaseColor);

	m_shader->SetInt("BaseColor", 0);
}

void RD_SimpleMaterial::UpdateSpecular() {
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_Specular);

	m_shader->SetInt("Specular", 2);

	m_shader->SetFloat("specularExp", m_Shininess);
}