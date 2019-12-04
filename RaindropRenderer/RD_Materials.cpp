#include "pch.h"
#include "RD_Materials.h"

//RD_SimpleMaterial

RD_SimpleMaterial::RD_SimpleMaterial(RD_ShaderLoader* shader, vec3f Color, vec3f SpecularColor, float SpecularExp) {
	m_shader = shader;

	BaseColor = Color;

	m_SpecularColor = SpecularColor;
	m_SpecularExp = SpecularExp;
}

RD_SimpleMaterial::~RD_SimpleMaterial() {
	delete m_shader;
}

void RD_SimpleMaterial::SetBaseColor(vec3f nColor) {
	BaseColor = nColor;

	UpdateColor();
}

void RD_SimpleMaterial::BindMaterial() {
	UpdateColor();
	UpdateSpecular();
}

void RD_SimpleMaterial::UpdateColor() {
	m_shader->SetVec3("BaseColor", BaseColor);
}

void RD_SimpleMaterial::UpdateSpecular() {
	m_shader->SetVec3("specularColor", m_SpecularColor);
	m_shader->SetFloat("specularExp", m_SpecularExp);
}