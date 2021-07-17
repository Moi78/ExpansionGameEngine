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
	vec2f currSize(m_game->GetRenderer()->getWindowHeigh(), m_game->GetRenderer()->getWindowWidth());
	if ((m_last_win_size.getX() == currSize.getX()) && (m_last_win_size.getX() == currSize.getY())) {
		return;
	}
	else {
		m_last_win_size = currSize;
	}

	m_game->GetRenderer()->ResizeViewport(
		vec2f((2.0f / 12.0f) * currSize.getY(), currSize.getX() * (2.0f / 12.0f)),
		vec2f(currSize.getY() * (10.0f / 12.0f), currSize.getX())
	);
}