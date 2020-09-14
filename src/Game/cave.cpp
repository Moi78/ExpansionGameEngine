#include "pch.h"
#include "cave.h"

EXPGE_LEVEL_CODE(cave);

cave::cave(EXP_Game* game, EXP_MapLoader* mloader) : EXP_Level(true, true) {
	m_game = game;

	m_character = new Character(game);

	m_gui_cache = new EXP_GUI_ColorCache(game, vec3f(0.0, 0.5, 1.0), 0.5f, 100.0f, 100.0f, 0.0f, 0.0f);

	m_gui_tex = new EXP_GUI_ImageTexture(game, "/noice.png", 0.5f, 200.0f, 200.0f, 100.0f, 100.0f);

	RaindropRenderer* rndr = game->GetRenderer();
	/*RD_Font* font = new RD_Font(rndr->GetFontRenderer(), "test", game->GetFilePathByRef("/font/font.otf"), 48);

	RD_TextBox* tb = new RD_TextBox(rndr, rndr->GetFontRenderer(), "H", "test", 20, 0, 0);*/
}

cave::~cave() {

}

void cave::OnStart() {

}

void cave::OnTick() {

}

void cave::Click() {
	m_gui_tex->SetPosition(vec3f(500, 300, 0));
}