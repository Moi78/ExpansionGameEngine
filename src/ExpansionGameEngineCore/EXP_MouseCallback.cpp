#include "EXP_MouseCallback.h"

EXP_MouseCallback::EXP_MouseCallback(int key, std::function<void()> callbck, bool repeat) {
    m_key = key;
    m_callbck = callbck;

    m_oldValue = false;
    m_repeat = repeat;
}

EXP_MouseCallback::~EXP_MouseCallback() {

}

void EXP_MouseCallback::Call(std::shared_ptr<RD_Windowing> win) {
    bool newValue = win->GetMouseButtonPress(m_key);
    if((newValue && m_repeat) || (newValue != m_oldValue)) {
        m_callbck();
    }

    if(m_up_cbck.has_value() && !newValue && m_oldValue) {
        m_up_cbck.value()();
    }

    m_oldValue = newValue;
}

void EXP_MouseCallback::AttachOnKeyUp(std::function<void()> cbck) {
    m_up_cbck = cbck;
}
