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
    EXP_MouseCallback(int key, std::function<void()> callbck, bool repeat = true);
    ~EXP_MouseCallback();

    void Call(std::shared_ptr<RD_Windowing> win);

    void AttachOnKeyUp(std::function<void()> cbck);

private:
    int m_key;

    std::function<void()> m_callbck;
    std::optional<std::function<void()>> m_up_cbck;

    bool m_oldValue;
    bool m_repeat;
};


#endif //EXPGE_EXP_MOUSECALLBACK_H
