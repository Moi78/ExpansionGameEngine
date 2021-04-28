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
#include "RD_Texture.h"

class RD_Mesh;

class RAINDROPRENDERER_API RD_ShaderMaterial {
public:
	RD_ShaderMaterial(RD_ShaderLoader*);
	~RD_ShaderMaterial();

	void AddTexture(std::string param, RD_Texture* tex);

	void BindMaterial();
	RD_ShaderLoader* GetShader();

	void RegisterMeshReference(RD_Mesh* msh);
	void UnregisterMeshReference(RD_Mesh* msh);
	void DrawMeshes();
private:
	RD_ShaderLoader* m_shader;
	std::vector<std::pair<std::string, RD_Texture*>> m_textures;

	std::vector<RD_Mesh*> m_meshes_references;
};

#endif