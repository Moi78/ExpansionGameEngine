#include "pch.h"
#include "RD_Materials.h"

//RD_SimpleMaterial

RD_SimpleMaterial::RD_SimpleMaterial(RD_ShaderLoader* shader, unsigned int BaseColor, vec3f SpecularColor, float Shininess, float SpecularStrength) {
	m_shader = shader;

	m_BaseColor = BaseColor;

	m_SpecularColor = SpecularColor;
	m_Shininess = Shininess;

	m_SpecularStrength = SpecularStrength;
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
	m_shader->SetVec3("specularColor", m_SpecularColor);
	m_shader->SetFloat("specularStrength", m_SpecularStrength);
	m_shader->SetFloat("specularExp", m_Shininess);
}