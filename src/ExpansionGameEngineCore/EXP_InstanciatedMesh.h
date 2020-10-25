#pragma once

#ifndef _EXP_INSTANCIATED_MESH_H__
#define _EXP_INSTANCIATED_MESH_H__

#ifdef _WIN32
	#ifdef EXPANSIONGAMEENGINECORE_EXPORTS
		#define EXPGE_API __declspec(dllexport)
	#else
		#define EXPGE_API __declspec(dllimport)
	#endif //EXPANSIONGAMEENGINE_CORE
#else
	#define EXPGE_API
#endif //_WIN32

#include "EXP_Component.h"
#include "EXP_Game.h"

#include <string>
#include <filesystem>
#include <iostream>

#include <RD_InstanciatedMesh.h>

class EXPGE_API EXP_InstanciatedMesh :
	public EXP_Component,
	public RD_InstanciatedMesh
{
public:
	EXP_InstanciatedMesh(EXP_Game* game, RD_ShaderMaterial* shader, std::string meshRef);
	~EXP_InstanciatedMesh();

	void LoadMesh(std::string meshRef);

	virtual void UseParentMatrix(mat4f parent) override;

private:
	EXP_Game* m_game;

	bool MeshRefExists(std::string ref);
};

#endif //_EXP_INSTANCIATED_MESH_H__