#ifndef EXPGE_EXP_KEYBOARDCALLBACK_H
#define EXPGE_EXP_KEYBOARDCALLBACK_H

#include <memory>

#include <RD_Callback.h>
#include <RD_API.h>

class EXP_KeyboardCallback {
public:
    EXP_KeyboardCallback(int key, std::function<void()> callbck);
    ~EXP_KeyboardCallback();

    void Call(std::shared_ptr<RD_Windowing> win);

private:
    int m_key;
    std::function<void()> m_callbck;
};


#endif //EXPGE_EXP_KEYBOARDCALLBACK_H
