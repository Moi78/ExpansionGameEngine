#pragma once

#include <EXP_Game.h>
#include <EXP_MapLoader.h>
#include <EXP_Level.h>

#include <EXP_GUI_ColorCache.h>
#include <EXP_GUI_ImageTexture.h>

#include "Character.h"

class cave : public EXP_Level
{
public:
	cave(EXP_Game* game, EXP_MapLoader* mloader);
	~cave();

	virtual void OnStart() override;
	virtual void OnTick() override;

private:
	EXP_Game* m_game;

	Character* m_character;

	EXP_GUI_ColorCache* m_gui_cache;
	EXP_GUI_ImageTexture* m_gui_tex;
};

EXPGE_LEVEL_HEADER(cave);