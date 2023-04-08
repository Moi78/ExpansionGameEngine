#include "EXP_WidgetBasic.h"

// EXP_GuiSolidRect

EXP_GuiSolidRect::EXP_GuiSolidRect(EXP_Game* game, EXP_GuiWidget* parent) : EXP_GuiWidget(parent) {
    m_mat = game->QueryMaterial("");
}

EXP_GuiSolidRect::~EXP_GuiSolidRect() {

}

void EXP_GuiSolidRect::RenderWidget() {

}