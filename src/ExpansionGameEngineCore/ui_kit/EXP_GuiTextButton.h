#ifndef EXPGE_EXP_GUITEXTBUTTON_H
#define EXPGE_EXP_GUITEXTBUTTON_H

#include "EXP_Game.h"
#include "EXP_GuiManager.h"
#include "EXP_WidgetBasic.h"

class EXP_GuiTextButton : public EXP_GuiWidget {
public:
    EXP_GuiTextButton(EXP_Game* game, RD_Rect pos, vec4 colorN, vec4 colorH, vec4 colorP, std::string text, std::shared_ptr<EXP_GuiWidget> parent = {});
    ~EXP_GuiTextButton();

    void RenderWidget(std::shared_ptr<RD_Quad> surface, const RD_Rect& parentRect, std::shared_ptr<RD_RenderSynchronizer> sync) override;
    void Event(vec2 curPos, bool leftButtonPress) override;

private:
    EXP_Game* m_game;
    std::shared_ptr<EXP_GuiWidget> m_parent;

    enum states_t {INIT, HOVER, CLICKED};
    states_t m_state;

    std::shared_ptr<EXP_GuiSolidRect> m_button_rect;
    std::shared_ptr<EXP_GuiTextStatic> m_button_text;

    vec4 m_colorN, m_colorH, m_colorP;

    bool m_oldMLB;
    bool m_clickFlag;
};


#endif //EXPGE_EXP_GUITEXTBUTTON_H
