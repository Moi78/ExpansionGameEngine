#include "pch.h"
#include "cave.h"

EXPGE_LEVEL_CODE(cave);

cave::cave(EXP_Game* game, EXP_MapLoader* mloader) : EXP_Level(true, true) {
	m_game = game;

	m_character = new Character(game);

	m_gui_cache = new EXP_GUI_ColorCache(game, vec3f(0.0, 0.5, 1.0), 0.5f, 100.0f, 100.0f, 0.0f, 0.0f);

	m_gui_tex = new EXP_GUI_ImageTexture(game, "/noice.png", 0.5f, 200.0f, 200.0f, 200.0f, 200.0f);
	EXP_GUI_ImageTexture* tex = new EXP_GUI_ImageTexture(game, "/noice.png", 1.0f, 50.0f, 50.0f, 500.0f, 300.0f);
}

cave::~cave() {

}

void cave::OnStart() {

}

void cave::OnTick() {
	float w = (float)m_game->GetRenderer()->getWindowWidth();
	float h = (float)m_game->GetRenderer()->getWindowHeigh();

	float mX = m_game->GetInputHandler()->GetGUI_SpaceMousePosX();
	float mY = m_game->GetInputHandler()->GetGUI_SpaceMousePosY();

	m_gui_tex->SetPosition(vec3f(mX, mY, 0.0f));
}