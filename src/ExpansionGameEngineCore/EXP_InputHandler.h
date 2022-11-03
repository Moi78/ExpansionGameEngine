#ifndef EXPGE_EXP_INPUTHANDLER_H
#define EXPGE_EXP_INPUTHANDLER_H

#include <memory>
#include <vector>

#include <RD_API.h>

#include "EXP_KeyboardCallback.h"
#include "EXP_MouseCallback.h"

class EXP_InputHandler {
public:
    EXP_InputHandler(std::shared_ptr<RD_Windowing> win);
    ~EXP_InputHandler() {};

    void UpdateAll();

    void SetCursorVisibility(bool visibility);
    void ResetCursor();

    void RegisterKeyboardCallback(std::shared_ptr<EXP_KeyboardCallback> kcbck);

    float GetCursorPosX();
    float GetCursorPosY();

private:
    std::shared_ptr<RD_Windowing> m_win;

    std::vector<std::shared_ptr<EXP_KeyboardCallback>> m_kcbcks;

    bool m_curHidden;
};


#endif //EXPGE_EXP_INPUTHANDLER_H
