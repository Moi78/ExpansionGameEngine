#include "pch.h"
#include "MaterialEditor.h"

EXPGE_LEVEL_CODE(MaterialEditor);

MaterialEditor::MaterialEditor(EXP_Game* game, EXP_MapLoader* mloader) : EXP_Level(true, true) {
	m_game = game;
	m_cam = new EXP_Camera(game, vec3f(-3.0f), vec3f(), vec3f(), vec3f(), 60.0f, 0.01f, 100.0f);
}

MaterialEditor::~MaterialEditor() {

}

void MaterialEditor::OnStart() {
	m_cam->Use();
}

void MaterialEditor::OnTick() {
	m_game->GetRenderer()->ResizeViewport(vec2f(0, 0), vec2f(m_game->GetRenderer()->getWindowWidth() / 2, m_game->GetRenderer()->getWindowHeigh()));
}