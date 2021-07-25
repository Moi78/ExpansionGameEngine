#pragma once

#ifndef _EXP_STATIC_MESH_H__
#define _EXP_STATIC_MESH_H__

#ifdef _WIN32
#ifdef EXPANSIONGAMEENGINECORE_EXPORTS
#define EXPGE_API __declspec(dllexport)
#else
#define EXPGE_API __declspec(dllimport)
#endif
#endif //_WIN32

#include "EXP_Component.h"
#include "EXP_Game.h"

#include <string>
#include <filesystem>
#include <iostream>

#include <RD_Mesh.h>

#include <BulldozerFileManager.h>

#include <mat4.h>

class EXPGE_API EXP_StaticMesh :
	public EXP_Component,
	public RD_Mesh
{
public:
	EXP_StaticMesh(
		EXP_Game* gameinstance,
		RD_ShaderMaterial* shader,
		std::string meshRef,
		vec3f pos, vec3f rot, vec3f scale,
		bool useSystemTree = false
	);

	~EXP_StaticMesh();

	void LoadMesh(std::string MeshRef);

	virtual void SetPosition(vec3f nPos) override;
	virtual void SetRotation(vec3f nRot) override;
	virtual void SetScale(vec3f nScale) override;
	
	virtual void UseParentMatrix(mat4f parent) override;

private:
	bool MeshRefExists(std::string MeshRef);

	EXP_Game* m_gameinstance;
	bool m_useSystemTree;
};

#endif //_EXP_STATIC_MESH__