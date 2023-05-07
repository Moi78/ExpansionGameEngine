#ifndef EXPGE_EXP_GUIBUTTON_H
#define EXPGE_EXP_GUIBUTTON_H

#include "EXP_Game.h"
#include "EXP_GuiManager.h"
#include "EXP_WidgetBasic.h"

#include "EXP_MouseCallback.h"

class EXP_GuiButton : public EXP_GuiWidget {
public:
    EXP_GuiButton(EXP_Game* game, RD_Rect pos, std::string texBase, std::string texPush, std::string texPress, std::shared_ptr<EXP_GuiWidget> parent = {});
    ~EXP_GuiButton();

    void RenderWidget(std::shared_ptr<RD_Quad> surface, const RD_Rect& parentRect, std::shared_ptr<RD_RenderSynchronizer> sync) override;
    void Event() override;

private:
    void RegisterClick();
    void UnregisterClick();

    EXP_Game* m_game;

    enum states_t {INIT, HOVER, CLICKED};
    states_t m_state;

    std::vector<std::shared_ptr<RD_Texture>> m_texs;
    std::shared_ptr<EXP_Material> m_mat;

    std::shared_ptr<EXP_MouseCallback> m_cbck;
};


#endif //EXPGE_EXP_GUIBUTTON_H
