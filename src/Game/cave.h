#pragma once

#include <EXP_Game.h>
#include <EXP_MapLoader.h>
#include <EXP_Level.h>

#include "Character.h"

class cave : public EXP_Level
{
public:
	cave(EXP_Game* game, EXP_MapLoader* mloader);
	~cave();

	virtual void OnStart() override;
	virtual void OnTick() override;

private:
	Character* m_character;
};

EXPGE_LEVEL_HEADER(cave);