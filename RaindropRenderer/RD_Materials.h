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
	RD_SimpleMaterial(BD_MatDef mat);
	~RD_SimpleMaterial();

	void BindMaterial(RD_ShaderLoader*);
private:
	void UpdateColor(RD_ShaderLoader*);
	void UpdateSpecular(RD_ShaderLoader*);
	void UpdateNormal(RD_ShaderLoader*);

	unsigned int m_BaseColor;
	unsigned int m_NormalMap;
	bool m_normalEnabled;

	unsigned int m_Specular;
	float m_Shininess;
};

#endif