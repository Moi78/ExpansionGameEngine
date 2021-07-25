#include "pch.h"
#include "Editor.h"

EXPGE_LEVEL_CODE(Editor);

Editor::Editor(EXP_Game* game, EXP_MapLoader* mloader) {
	m_game = game;
	m_default_cam = new CameraClass(game);
	m_last_win_size = vec2f(0.0f, 0.0f);
}

Editor::~Editor() {

}

void Editor::OnStart() {

}

void Editor::OnTick() {

}