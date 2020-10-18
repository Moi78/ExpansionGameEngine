#pragma once

#include <mat4.h>
#include <vec4.h>

#include <EXP_Game.h>
#include <EXP_MapLoader.h>
#include <EXP_Level.h>

#include <EXP_GUI_ColorCache.h>
#include <EXP_GUI_ImageTexture.h>
#include <EXP_GUI_Button.h>

#include <EXP_Callbacks.h>
#include <EXP_RigidBody.h>

#include <RD_InstanciatedMesh.h>

#include "Character.h"

class cave : public EXP_Level
{
public:
	cave(EXP_Game* game, EXP_MapLoader* mloader);
	~cave();

	virtual void OnStart() override;
	virtual void OnTick() override;

	void Click();

private:
	EXP_Game* m_game;
	EXP_MapLoader* m_mloader;

	Character* m_character;

	EXP_GUI_ColorCache* m_gui_cache;
	EXP_GUI_ImageTexture* m_gui_tex;

	EXP_MouseButtonCallback* m_mcllbck;

	EXP_InstanciatedMesh* m_meshes;

	EXP_RB_Box* m_floor;
};

EXPGE_LEVEL_HEADER(cave);