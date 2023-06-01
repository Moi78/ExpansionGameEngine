#ifndef EXPGE_UI_H
#define EXPGE_UI_H

#include <EXP_Game.h>
#include <EXP_WidgetBasic.h>
#include <ui_kit/EXP_GuiWindow.h>

namespace Ombrage {
    class UI {
    public:
        UI(EXP_Game *game);
        ~UI();

        void ContructUI();

    private:
        EXP_Game* m_game;

        std::shared_ptr<EXP_GuiWindow> m_light_control;
        std::shared_ptr<EXP_GuiWindow> m_file_control;
        std::shared_ptr<EXP_GuiWindow> m_work_area;
    };
}


#endif //EXPGE_UI_H
