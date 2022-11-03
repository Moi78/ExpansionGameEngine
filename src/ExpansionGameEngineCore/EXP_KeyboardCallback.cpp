#include "EXP_KeyboardCallback.h"

EXP_KeyboardCallback::EXP_KeyboardCallback(int key, std::function<void()> callbck) {
    m_callbck = callbck;
    m_key = key;
}

EXP_KeyboardCallback::~EXP_KeyboardCallback() {

}

void EXP_KeyboardCallback::Call(std::shared_ptr<RD_Windowing> win) {
    if(win->GetKeyPress(m_key)) {
        m_callbck();
    }
}