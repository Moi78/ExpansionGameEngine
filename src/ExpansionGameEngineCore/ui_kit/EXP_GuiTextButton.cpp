#include "EXP_GuiTextButton.h"

EXP_GuiTextButton::EXP_GuiTextButton(EXP_Game* game,
                             RD_Rect pos,
                             vec4 colorN, vec4 colorH, vec4 colorP,
                             std::string text,
                             std::shared_ptr<EXP_GuiWidget> parent
) : EXP_GuiWidget(parent) {
    m_game = game;
    m_parent = parent;

    m_state = states_t::INIT;

    m_oldMLB = false;
    m_clickFlag = false;

    m_colorN = colorN;
    m_colorH = colorH;
    m_colorP = colorP;

    RD_Rect rectBG = {
            0, 0, pos.w, pos.h
    };

    RD_Rect rectTxt = rectBG;
    rectTxt.x = 5;
    rectTxt.y = (30 / 4) - 2;

    m_button_rect = std::make_shared<EXP_GuiSolidRect>(game, rectBG, colorN);
    AddChild(m_button_rect.get());

    m_button_text = std::make_shared<EXP_GuiTextStatic>(game, rectTxt, game->GetEngineFont(), vec4(1.0f, 1.0f, 1.0f, 1.0f));
    AddChild(m_button_text.get());

    m_button_text->ConstructText(text, 15);

    m_rect = pos;
}

EXP_GuiTextButton::~EXP_GuiTextButton() {

}

void EXP_GuiTextButton::RenderWidget(std::shared_ptr<RD_Quad> surface, const RD_Rect &parentRect,
                                 std::shared_ptr<RD_RenderSynchronizer> sync) {
}

void EXP_GuiTextButton::Event(vec2 curPos, bool leftButtonPress) {
    auto guiman = m_game->GetGuiManager();

    RD_Rect transRect{m_rect};
    if(m_parent.use_count()) {
        auto parentRect = m_parent->GetRect();
        transRect.x += parentRect.x;
        transRect.y += parentRect.y;
    }

    switch(m_state) {
        case states_t::INIT:
            if(isPointInRect(curPos, transRect)) {
                m_state = states_t::HOVER;

                m_button_rect->SetColor(m_colorH);
                guiman->SetRedrawFlag();
            }

            break;

        case states_t::HOVER:
            if(!isPointInRect(curPos, transRect)) {
                m_state = states_t::INIT;

                m_button_rect->SetColor(m_colorN);
                guiman->SetRedrawFlag();
            } else {
                if(leftButtonPress) {
                    m_state = states_t::CLICKED;

                    m_button_rect->SetColor(m_colorP);
                    guiman->SetRedrawFlag();
                }
            }

            break;

        case states_t::CLICKED:
            if(!leftButtonPress) {
                m_state = states_t::HOVER;

                m_button_rect->SetColor(m_colorH);
                guiman->SetRedrawFlag();
            }

            break;
    }

    m_oldMLB = leftButtonPress;
}