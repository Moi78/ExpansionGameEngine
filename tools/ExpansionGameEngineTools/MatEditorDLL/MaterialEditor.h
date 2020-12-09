#pragma once

#include <EXP_Game.h>
#include <EXP_Level.h>
#include <EXP_MapLoader.h>

#include <EXP_Camera.h>

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

	EXP_Camera* m_cam;
};

EXPGE_LEVEL_HEADER(MaterialEditor);