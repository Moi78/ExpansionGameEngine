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

#include <json/json.h>

#include "EXP_Game.h"
#include "EXP_StaticMesh.h"

class EXP_MapLoader
{
public:
	EXP_MapLoader(std::string mapFile);
	~EXP_MapLoader();

	void LoadMap();
	void UnloadMap();
};

#endif