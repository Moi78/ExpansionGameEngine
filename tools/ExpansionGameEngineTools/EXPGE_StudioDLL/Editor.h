#pragma once
#include <EXP_Game.h>
#include <EXP_Level.h>
#include <EXP_MapLoader.h>
#include <EXP_InputHandler.h>
#include <vec2.h>

#include "CameraClass.h"

class Editor : public EXP_Level
{
public:
	Editor(EXP_Game* game, EXP_MapLoader* mloader);
	~Editor();

	virtual void OnStart();
	virtual void OnTick();

private:
	EXP_Game* m_game;
	vec2f m_last_win_size;

	CameraClass* m_default_cam;
};

EXPGE_LEVEL_HEADER(Editor);