#ifndef EXPGE_UI_H
#define EXPGE_UI_H

#include <EXP_Game.h>

namespace Ombrage {
    class UI {
    public:
        UI(EXP_Game *game);
        ~UI();

        void ContructUI();

    private:
        EXP_Game* m_game;
    };
}


#endif //EXPGE_UI_H
