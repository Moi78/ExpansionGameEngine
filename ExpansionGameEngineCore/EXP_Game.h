#pragma once

#ifndef _EXP_GAME_H__
#define _EXP_GAME_H__

#ifdef EXPANSIONGAMEENGINECORE_EXPORTS
#define EXPGE_API __declspec(dllexport)
#else
#define EXPGE_API __declspec(dllimport)
#endif

#ifdef _DEBUG
#define RENDER_DBG true
#else
#define RENDER_DBG false
#endif

#include <iostream>
#include <string>
#include <vector>

#include <thread>
#include <mutex>

#include <RaindropRenderer.h>
#include <RD_Mesh.h>
#include <RD_PointLight.h>
#include <RD_Camera.h>

#include <BD_StructMan.h>

#include <vec3.h>

class EXPGE_API EXP_Game
{
public:
	EXP_Game(BD_Resolution res, BD_GameInfo gameinfo, vec3f refreshColor, std::string gameName);
	~EXP_Game();

	void LaunchMainLoop();
	
	BD_GameInfo GetGameInfo();

	//For internal usage only
	void RegisterMesh(RD_Mesh*);
	void RegisterPointLight(RD_PointLight*);

	RaindropRenderer* GetRenderer();
	vec3f GetRefreshColor();
	std::vector<RD_Mesh*> GetStaticMeshes();

	void MainLoop();

private:
	void InitGame(BD_Resolution winRes, vec3f refreshColor, std::string gameName, BD_GameInfo gameinfo);

	RaindropRenderer* m_rndr;

	BD_Resolution m_res;
	BD_GameInfo m_gameinfo;

	vec3f m_refreshColor;
	std::string m_gameName;

	//Renderables
	std::vector<RD_Mesh*> m_staticMeshes;
	std::vector<RD_PointLight*> m_points_light;
};

#endif //_EXP_GAME_H__