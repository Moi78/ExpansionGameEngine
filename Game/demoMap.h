#pragma once

#ifdef _WIN32
#define GAME_API __declspec(dllexport)
#endif

#include <iostream>

#include <EXP_Game.h>
#include <EXP_Level.h>
#include <EXP_MapLoader.h>
#include <EXP_HotLoad.h>

#include <EXP_Camera.h>

class demoMap : public EXP_Level
{
public:
	demoMap(EXP_Game*, EXP_MapLoader*);

	virtual void OnStart() override;
	virtual void OnTick() override;

private:
	EXP_Game* m_game;

	EXP_Camera* cam;
};

EXPGE_LEVEL_HEADER(demoMap);