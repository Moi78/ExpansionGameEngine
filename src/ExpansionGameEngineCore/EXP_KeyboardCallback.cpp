#include "EXP_KeyboardCallback.h"

EXP_KeyboardCallback::EXP_KeyboardCallback(int key, std::function<void()> callbck, bool repeatSig) {
    m_callbck = callbck;
    m_key = key;

    m_repeat_sig = repeatSig;

    m_new = false;
    m_old = false;
}

EXP_KeyboardCallback::~EXP_KeyboardCallback() {

}

void EXP_KeyboardCallback::Call(std::shared_ptr<RD_Windowing> win) {
    if(m_repeat_sig) {
        if (win->GetKeyPress(m_key)) {
            m_callbck();
        }
    } else {
        m_new = win->GetKeyPress(m_key);

        if((m_new != m_old) && m_new) {
            m_callbck();
        }

        m_old = m_new;
    }
}