#ifndef EXPGE_EXP_GUIBUTTON_H
#define EXPGE_EXP_GUIBUTTON_H

#include "EXP_Game.h"
#include "EXP_GuiManager.h"
#include "EXP_WidgetBasic.h"

class EXP_GuiButton : public EXP_GuiWidget {
public:
    EXP_GuiButton(EXP_Game* game, RD_Rect pos, std::string texBase, std::string texHover, std::string texPress, std::shared_ptr<EXP_GuiWidget> parent = {});
    ~EXP_GuiButton();

    void RenderWidget(std::shared_ptr<RD_Quad> surface, const RD_Rect& parentRect, std::shared_ptr<RD_RenderSynchronizer> sync) override;
    void Event(vec2 curPos, bool leftButtonPress) override;

private:
    EXP_Game* m_game;
    std::shared_ptr<EXP_GuiWidget> m_parent;

    enum states_t {INIT, HOVER, CLICKED};
    states_t m_state;

    std::vector<std::shared_ptr<RD_Texture>> m_texs;
    std::shared_ptr<EXP_Material> m_mat;

    bool m_oldMLB;
    bool m_clickFlag;
};


#endif //EXPGE_EXP_GUIBUTTON_H