#pragma once

#ifndef _RD_MATERIALS_H__
#define _RD_MATERIALS_H__

#ifdef _WIN32
#ifdef RAINDROPRENDERER_EXPORTS
#define RAINDROPRENDERER_API __declspec(dllexport)
#else
#define RAINDROPRENDERER_API __declspec(dllimport)
#endif

#else

#define RAINDROPRENDERER_API

#endif //_WIN32

#include "vec3.h"
#include "RD_ShaderLoader.h"

#include <BD_StructMan.h>

class RAINDROPRENDERER_API RD_SimpleMaterial
{
public:
	RD_SimpleMaterial(RD_ShaderLoader* shader, BD_MatDef mat);
	~RD_SimpleMaterial();

	void SetBaseColor(unsigned int);

	void BindMaterial();
private:
	void UpdateColor();
	void UpdateSpecular();

	unsigned int m_BaseColor;

	unsigned int m_Specular;
	float m_Shininess;

	RD_ShaderLoader* m_shader;
};

#endif