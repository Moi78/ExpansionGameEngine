#pragma once

#ifndef _EXP_MAP_LOADER_H__
#define _EXP_MAP_LOADER_H__

#ifdef _WIN32
	#ifdef EXPANSIONGAMEENGINECORE_EXPORTS
		#define EXPGE_API __declspec(dllexport)
	#else
		#define EXPGE_API __declspec(dllimport)
	#endif
#else
	#define EXPGE_API
#endif

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>

#include <vector>

#include <json/json.h>

#include "EXP_Game.h"
#include "EXP_StaticMesh.h"
#include "EXP_PointLight.h"
#include "EXP_DirLight.h"
#include "EXP_HotLoad.h"

class EXP_Level;

class EXPGE_API EXP_MapLoader
{
public:
	EXP_MapLoader(EXP_Game* game, std::string mapFile);
	~EXP_MapLoader();

	bool LoadMap();
	void UnloadMap();

	void UpdateLevel();

	EXP_StaticMesh* GetStaticMeshByName(std::string name);
	EXP_PointLight* GetPointLightByName(std::string name);
	EXP_DirLight* GetDirLightByName(std::string name);

private:
	EXP_Game* m_game;

	std::vector<EXP_StaticMesh*> m_meshes;
	std::vector<EXP_PointLight*> m_ptlights;
	std::vector<EXP_DirLight*> m_dlights;

	std::ifstream m_map;
	std::string m_mapRef;

	EXP_Level* m_levelCode;
};

TYPEDEF(EXP_Level*, LEVELCODEHANDLER, EXP_Game*, EXP_MapLoader*);

#endif