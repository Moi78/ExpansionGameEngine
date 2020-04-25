#pragma once

#ifndef _MAP1_H__
#define _MAP1_H__

#ifdef _WIN32
	#ifdef GAME_EXPORTS
		#define GAME_API __declspec(dllexport)
	#else
		#define GAME_API __declspec(dllimport)
	#endif
#else
	#define GAME_API
#endif

#include <iostream>

#include <EXP_Game.h>
#include <EXP_HotLoad.h>
#include <EXP_Level.h>
#include <EXP_MapLoader.h>

#include <EXP_Camera.h>

class GAME_API Map1 : public EXP_Level
{
public:
	Map1(EXP_Game* game, EXP_MapLoader* mloader);
	~Map1();

	virtual void OnStart() override;
	virtual void OnTick() override;

private:
	EXP_Camera* m_cam;
};

EXPGE_LEVEL_HEADER(Map1);

#endif