#pragma once

#include <EXP_Game.h>
#include <EXP_Level.h>
#include <EXP_MapLoader.h>
#include <EXP_InputHandler.h>

#include <EXP_Camera.h>
#include <EXP_Callbacks.h>

#include <GLFW/glfw3.h>

class MaterialEditor : 
	public EXP_Level
{
public:
	MaterialEditor(EXP_Game* game, EXP_MapLoader* mloader);
	~MaterialEditor();

	virtual void OnStart() override;
	virtual void OnTick() override;

private:
	EXP_Game* m_game;
	EXP_MapLoader* m_loader;

	EXP_MouseButtonCallback* m_clickev;
	vec2f m_last_win_size;

	EXP_Camera* m_cam;
};

EXPGE_LEVEL_HEADER(MaterialEditor);