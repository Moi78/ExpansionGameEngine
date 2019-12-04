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
	RD_SimpleMaterial(RD_ShaderLoader* shader, vec3f Color = vec3f(1.0f, 0.0f, 0.0f), vec3f SpecularColor = vec3f(1.0f, 0.0f, 0.0f), float SpecularExp = 16.0f);
	~RD_SimpleMaterial();

	void SetBaseColor(vec3f);

	void BindMaterial();
private:
	void UpdateColor();
	void UpdateSpecular();

	vec3f BaseColor;

	vec3f m_SpecularColor;
	float m_SpecularExp;

	RD_ShaderLoader* m_shader;
};

#endif