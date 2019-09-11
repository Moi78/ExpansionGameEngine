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
	RD_SimpleMaterial(RD_ShaderLoader* shader, vec3f Color = vec3f(1.0f, 0.0f, 0.0f));
	~RD_SimpleMaterial();

	void SetBaseColor(vec3f);

	void BindMaterial();
private:
	void UpdateColor();

	vec3f BaseColor;
	RD_ShaderLoader* m_shader;
};

#endif