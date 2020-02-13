#pragma once

#ifndef _EXP_STATIC_MESH_H__
#define _EXP_STATIC_MESH_H__

#ifdef EXPANSIONGAMEENGINECORE_EXPORTS
#define EXPGE_API __declspec(dllexport)
#else
#define EXPGE_API __declspec(dllimport)
#endif

#include "EXP_Component.h"
#include "EXP_Game.h"

#include <string>
#include <filesystem>
#include <iostream>

#include <RD_Mesh.h>

#include <BulldozerFileManager.h>

class EXPGE_API EXP_StaticMesh : public EXP_Component
{
public:
	EXP_StaticMesh(EXP_Game* gameinstance, std::string meshRef, BD_MatDef material, vec3f pos, vec3f rot, vec3f scale);
	~EXP_StaticMesh();

	//For internal usage only
	RD_Mesh* GetRawMeshData();

	bool MeshRefExists(std::string MeshRef);
	void LoadMesh(std::string MeshRef);

	void Update();

private:
	RD_Mesh* m_RawMesh;
	BD_MatDef m_material;
	EXP_Game* m_gameinstance;
};

#endif //_EXP_STATIC_MESH__