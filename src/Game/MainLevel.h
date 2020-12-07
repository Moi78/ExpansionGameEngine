#pragma once

#include <EXP_Game.h>
#include <EXP_MapLoader.h>
#include <EXP_Level.h>
#include <EXP_Callbacks.h>

#include <RD_MeshBuilder.h>

#include "Character.h"

class MainLevel : public EXP_Level
{
public:
	MainLevel(EXP_Game* game, EXP_MapLoader* mloader);
	~MainLevel();

	virtual void OnStart() override;
	virtual void OnTick() override;

	void ChangeLVL();

private:
	EXP_Game* m_game;

	EXP_KeyboardCallback* m_ch_lvl;

	Character* m_player;
};

EXPGE_LEVEL_HEADER(MainLevel);