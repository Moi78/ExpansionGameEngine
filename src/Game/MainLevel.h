#pragma once

#include <EXP_Game.h>
#include <EXP_MapLoader.h>
#include <EXP_Level.h>

#include "Character.h"

class MainLevel : public EXP_Level
{
public:
	MainLevel(EXP_Game* game, EXP_MapLoader* mloader);
	~MainLevel();

	virtual void OnStart() override;
	virtual void OnTick() override;

private:
	EXP_Game* m_game;

	Character* m_player;
};

EXPGE_LEVEL_HEADER(MainLevel);