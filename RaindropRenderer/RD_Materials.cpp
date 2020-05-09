#include "pch.h"
#include "RD_Materials.h"

//RD_SimpleMaterial

RD_SimpleMaterial::RD_SimpleMaterial(BD_MatDef mdef) {
	m_BaseColor = mdef.BaseColor;

	m_NormalMap = mdef.NormalMap;
	m_normalEnabled = mdef.NormalEnabled;

	m_Specular = mdef.Specular;
	m_Shininess = mdef.Shininess;
}

RD_SimpleMaterial::~RD_SimpleMaterial() {

}

void RD_SimpleMaterial::BindMaterial(RD_ShaderLoader* shader) {
	UpdateColor(shader);
	UpdateSpecular(shader);
	UpdateNormal(shader);
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

void RD_SimpleMaterial::UpdateNormal(RD_ShaderLoader* shader) {
	if (m_normalEnabled) {
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_NormalMap);

		shader->SetInt("NormalMap", 3);
		shader->SetBool("NormalEnabled", true);
	}
	else {
		shader->SetBool("NormalEnabled", false);
	}
}