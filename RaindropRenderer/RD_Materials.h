#pragma once

#ifndef _RD_MATERIALS_H__
#define _RD_MATERIALS_H__

#ifdef RAINDROPRENDERER_EXPORTS
#define RAINDROPRENDERER_API __declspec(dllexport)
#else
#define RAINDROPRENDERER_API __declspec(dllimport)
#endif

#include "vec3.h"
#include "RD_ShaderLoader.h"

class RAINDROPRENDERER_API RD_SimpleMaterial
{
public:
	RD_SimpleMaterial(RD_ShaderLoader* shader, unsigned int BaseColor, vec3f SpecularColor = vec3f(1.0f, 0.0f, 0.0f), float SpecularExp = 16.0f);
	~RD_SimpleMaterial();

	void SetBaseColor(unsigned int);

	void BindMaterial();
private:
	void UpdateColor();
	void UpdateSpecular();

	unsigned int m_BaseColor;

	vec3f m_SpecularColor;
	float m_SpecularExp;

	RD_ShaderLoader* m_shader;
};

#endif