#pragma once

#ifdef _WIN32
#define GAME_API __declspec(dllexport)
#endif

#include <EXP_Game.h>
#include <EXP_Level.h>
#include <EXP_HotLoad.h>
#include <EXP_InputHandler.h>

#include <iostream>

class map2 : public EXP_Level
{
public:
	map2(EXP_Game* game, EXP_MapLoader* mloader);
	~map2();

	virtual void OnStart();
	virtual void OnTick();
};

EXPGE_LEVEL_HEADER(map2);