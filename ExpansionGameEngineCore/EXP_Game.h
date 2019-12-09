#pragma once

#ifndef _EXP_GAME_H__
#define _EXP_GAME_H__

#ifdef EXPANSIONGAMEENGINECORE_EXPORTS
#define EXPGE_API __declspec(dllexport)
#else
#define EXPGE_API __declspec(dllimport)
#endif

#include <iostream>
#include <string>

#include <RaindropRenderer.h>

#include <BD_StructMan.h>

#include <vec3.h>

class EXPGE_API EXP_Game
{
public:
	EXP_Game(BD_Resolution res, BD_GameInfo gameinfo, vec3f refreshColor, std::string gameName);
	~EXP_Game();

	void LaunchMainLoop();
	
	BD_GameInfo GetGameInfo();
private:
	void InitGame(BD_Resolution winRes, vec3f refreshColor, std::string gameName, BD_GameInfo gameinfo);

	RaindropRenderer* m_rndr;

	BD_Resolution m_res;
	BD_GameInfo m_gameinfo;

	vec3f m_refreshColor;
	std::string m_gameName;
};

#endif //_EXP_GAME_H__