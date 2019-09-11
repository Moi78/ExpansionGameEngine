#include "pch.h"
#include "RD_Materials.h"

//RD_SimpleMaterial

RD_SimpleMaterial::RD_SimpleMaterial(RD_ShaderLoader* shader, vec3f Color) {
	m_shader = shader;
	BaseColor = Color;
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
}

void RD_SimpleMaterial::UpdateColor() {
	m_shader->SetVec3("BaseColor", BaseColor);
}