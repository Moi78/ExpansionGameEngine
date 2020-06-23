#pragma once

#ifndef _EXP_MAP_LOADER_H__
#define _EXP_MAP_LOADER_H__

#ifdef _WIN32
	#ifdef EXPANSIONGAMEENGINECORE_EXPORTS
		#define EXPGE_API __declspec(dllexport)
	#else
		#define EXPGE_API __declspec(dllimport)
	#endif
	
	#include <json/json.h>
#else
	#define EXPGE_API
    #include <jsoncpp/json/json.h>
#endif

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>
#include <thread>
#include <memory>

#include "EXP_Game.h"
#include "EXP_StaticMesh.h"
#include "EXP_PointLight.h"
#include "EXP_DirLight.h"
#include "EXP_HotLoad.h"

class EXP_Level;

TYPEDEF(EXP_Level*, LEVELCODEHANDLER, EXP_Game*, EXP_MapLoader*);
TYPEDEF(void, LEVELCODERELEASER, void*);

class EXPGE_API EXP_MapLoader
{
public:
	EXP_MapLoader(EXP_Game* game);
	~EXP_MapLoader();

	bool LoadMap(std::string);
	void UnloadMap();

	EXP_Level* GetLevelCode();

	EXP_StaticMesh* GetStaticMeshByName(std::string name);
	EXP_PointLight* GetPointLightByName(std::string name);
	EXP_DirLight* GetDirLightByName(std::string name);

private:
	EXP_Game* m_game;

	std::vector<EXP_StaticMesh*> m_meshes;
	std::vector<EXP_PointLight*> m_ptlights;
	std::vector<EXP_DirLight*> m_dlights;

	EXP_Level* m_levelCode;

	LEVELCODERELEASER m_rl;
	void* m_rawLevelCode;

	std::mutex m_mu;
};

#endif
