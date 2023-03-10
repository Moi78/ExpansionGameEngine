#include "EXP_GuiManager.h"

EXP_GuiWidget::EXP_GuiWidget(EXP_GuiWidget *parent) {
    if(parent) {
        m_parent = parent;
        m_parent->SetChild(this);
    } else {
        m_parent = nullptr;
    }

    m_child = nullptr;
    m_rect = {0, 0, 0, 0};
}

void EXP_GuiWidget::SetChild(EXP_GuiWidget *child) {
    m_child = child;
}

void EXP_GuiWidget::Paint() {
    RenderWidget();

    if(m_child) {
        m_child->Paint();
    }
}

const RD_Rect& EXP_GuiWidget::GetRect() {
    return m_rect;
}

//-------------------------------------------------------------------

EXP_GuiManager::EXP_GuiManager(std::shared_ptr<RD_API> api) {
    m_api = api;
}
