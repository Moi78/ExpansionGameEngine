#ifndef EXPGE_EXP_MOUSECALLBACK_H
#define EXPGE_EXP_MOUSECALLBACK_H

#define MOUSE_BUTTON_LEFT 0
#define MOUSE_BUTTON_RIGHT 1
#define MOUSE_BUTTON_MIDDLE 2

#include <memory>

#include <RD_Callback.h>
#include <RD_API.h>

class EXP_MouseCallback {
public:
    EXP_MouseCallback(int key, std::function<void()> callbck);
    ~EXP_MouseCallback();

    void Call(std::shared_ptr<RD_Windowing> win);

private:
    int m_key;
    std::function<void()> m_callbck;
};


#endif //EXPGE_EXP_MOUSECALLBACK_H
