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
#include "RD_RenderingAPI.h"
#include "RaindropRenderer.h"

class RD_Mesh;

class RAINDROPRENDERER_API RD_ShaderMaterial {
public:
	RD_ShaderMaterial(RD_ShaderLoader* shader, RaindropRenderer* api);
	~RD_ShaderMaterial();

	void AddTexture(std::string param, RD_Texture* tex);
	void PurgeTextures();
	void MakeSSBO();

	void BindMaterial();
	RD_ShaderLoader* GetShader();
	void SetShader(RD_ShaderLoader* shader, bool nodelete);

	void RegisterMeshReference(RD_Mesh* msh);
	void UnregisterMeshReference(RD_Mesh* msh);
	void UnregisterAllMeshes();

	void DrawMeshes();
	void DrawMeshesShadow();

	void DeleteMaterial();

	void SetMetaInf(const std::string& meta);
	std::string GetMetaInf();

private:
	RaindropRenderer* m_rndr;

	RD_ShaderLoader* m_shader;
	RD_ShaderStorageBuffer* m_texture_buffer;
	std::vector<std::pair<std::string, RD_Texture*>> m_textures;

	std::vector<RD_Mesh*> m_meshes_references;

	std::string m_meta;
};

#endif