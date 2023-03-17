#include "EXP_GuiManager.h"

EXP_GuiWidget::EXP_GuiWidget(EXP_GuiWidget *parent) {
    if(parent) {
        m_parent = parent;
        m_parent->AddChild(this);
    } else {
        m_parent = nullptr;
    }

    m_rect = {0, 0, 0, 0};
}

void EXP_GuiWidget::AddChild(EXP_GuiWidget *child) {
    m_child.push_back(child);
}

void EXP_GuiWidget::Paint() {
    RenderWidget();

    for(auto& c : m_child) {
        c->Paint();
    }
}

const RD_Rect& EXP_GuiWidget::GetRect() {
    return m_rect;
}

//-------------------------------------------------------------------

EXP_GuiManager::EXP_GuiManager(std::shared_ptr<RD_API> api) {
    m_api = api;
}

void EXP_GuiManager::AddWidget(std::shared_ptr<EXP_GuiWidget> widget) {
    m_widgets.push_back(widget);
}

void EXP_GuiManager::RenderGui() {
    for(auto& w : m_widgets) {
        w->Paint();
    }
}
