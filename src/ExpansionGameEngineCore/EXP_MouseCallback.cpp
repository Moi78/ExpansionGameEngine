#include "EXP_MouseCallback.h"

EXP_MouseCallback::EXP_MouseCallback(int key, std::function<void()> callbck) {
    m_key = key;
    m_callbck = callbck;
}

EXP_MouseCallback::~EXP_MouseCallback() {

}

void EXP_MouseCallback::Call(std::shared_ptr<RD_Windowing> win) {
    if(win->GetMouseButtonPress(m_key)) {
        m_callbck();
    }
}