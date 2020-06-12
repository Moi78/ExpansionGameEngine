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

#include <vector>

#include "vec3.h"
#include "RD_ShaderLoader.h"

#include <BD_StructMan.h>

class RAINDROPRENDERER_API RD_ShaderMaterial {
public:
	RD_ShaderMaterial(RD_ShaderLoader*);
	~RD_ShaderMaterial();

	void AddTexture(std::string param, unsigned int id);

	void BindMaterial();
	RD_ShaderLoader* GetShader();
private:
	RD_ShaderLoader* m_shader;
	std::vector < std::pair<std::string, unsigned int> > m_textures;
};

#endif