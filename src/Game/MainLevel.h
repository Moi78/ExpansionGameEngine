#pragma once

#include <EXP_Game.h>
#include <EXP_MapLoader.h>
#include <EXP_Level.h>
#include <EXP_Callbacks.h>
#include <EXP_RigidBody.h>
#include <EXP_PostProcessEffect.h>

#include <EXP_TextSurface.h>

#include <EXP_Particles.h>
#include <EXP_RigidBody.h>

#include "Character.h"

#include <RD_ReflectionCapture.h>

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
	EXP_MapLoader* m_mloader;

	EXP_RB_Box* m_floor;
	EXP_RB_Sphere* m_test;
	EXP_KeyboardCallback* m_ch_lvl;

	EXP_TextSurface* m_txt;

	Character* m_player;

	RD_ReflectionCapture* m_cap;
};

EXPGE_LEVEL_HEADER(MainLevel);