#include "EXP_InputHandler.h"

EXP_InputHandler::EXP_InputHandler(std::shared_ptr<RD_Windowing> win) {
    m_win = win;

    m_curHidden = false;
}

void EXP_InputHandler::SetCursorVisibility(bool visibility) {
    m_win->SetCursorVisibility(visibility);
    m_curHidden = !visibility;
}

void EXP_InputHandler::ResetCursor() {
    if(m_curHidden) {
        m_win->SetCursorPosition(0, 0);
    }
}

void EXP_InputHandler::RegisterKeyboardCallback(std::shared_ptr <EXP_KeyboardCallback> kcbck) {
    m_kcbcks.push_back(kcbck);
}

void EXP_InputHandler::RegisterMouseCallback(std::shared_ptr<EXP_MouseCallback> mcbck) {
    m_mcbcks.push_back(mcbck);
}

void EXP_InputHandler::UpdateAll() {
    for(auto& k : m_kcbcks) {
        k->Call(m_win);
    }

    for(auto& m : m_mcbcks) {
        m->Call(m_win);
    }
}

float EXP_InputHandler::GetCursorPosX() {
    return m_win->GetCursorPositionX();
}

float EXP_InputHandler::GetCursorPosY() {
    return m_win->GetCursorPositionY();
}