#ifndef EXPGE_EXP_KEYBOARDCALLBACK_H
#define EXPGE_EXP_KEYBOARDCALLBACK_H

#include <memory>

#include <RD_Callback.h>
#include <RD_API.h>

class EXP_KeyboardCallback {
public:
    EXP_KeyboardCallback(int key, std::function<void()> callbck, bool repeatSig = true);
    ~EXP_KeyboardCallback();

    void Call(std::shared_ptr<RD_Windowing> win);

private:
    int m_key;
    bool m_repeat_sig;

    bool m_new;
    bool m_old;

    std::function<void()> m_callbck;
};


#endif //EXPGE_EXP_KEYBOARDCALLBACK_H
