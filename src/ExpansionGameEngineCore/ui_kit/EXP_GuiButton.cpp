#include "EXP_GuiButton.h"

EXP_GuiButton::EXP_GuiButton(EXP_Game* game,
                             RD_Rect pos,
                             std::string texBase, std::string texPush, std::string texPress,
                             std::shared_ptr<EXP_GuiWidget> parent
) {
    m_game = game;

    m_state = states_t::INIT;

    for(int i = 0; i < 3; i++) {
        m_texs.push_back(game->GetRenderer()->GetAPI()->CreateTexture());
    }

    m_texs[0]->LoadTextureFromFile(game->GetGameContentPath() + texBase);
    m_texs[1]->LoadTextureFromFile(game->GetGameContentPath() + texPush);
    m_texs[2]->LoadTextureFromFile(game->GetGameContentPath() + texPress);

    m_mat = game->QueryMaterial("/ui/materials/button.json", true, true, false);
    m_mat->GetPipeline()->RegisterTextureArray(m_texs, 3);
    m_mat->GetPipeline()->BuildPipeline();

    m_cbck = std::make_shared<EXP_MouseCallback>(GLFW_MOUSE_BUTTON_LEFT, CL_VDFUNCPTR(EXP_GuiButton::RegisterClick));
    m_cbck->AttachOnKeyUp(CL_VDFUNCPTR(EXP_GuiButton::UnregisterClick));
    m_game->GetInputHandler()->RegisterMouseCallback(m_cbck);

    m_rect = pos;
}

EXP_GuiButton::~EXP_GuiButton() {

}

void EXP_GuiButton::RenderWidget(std::shared_ptr<RD_Quad> surface, const RD_Rect &parentRect,
                                 std::shared_ptr<RD_RenderSynchronizer> sync) {
    auto pline = m_mat->GetPipeline();

    RD_Rect nrect{m_rect};
    nrect.x += parentRect.x;
    nrect.y += parentRect.y;

    vec4 zero = vec4{0.0f, 0.0f, 0.0f, 0.0f};
    uint32_t id = static_cast<uint32_t>(m_state);

    pline->Bind(sync);
    pline->PartialPushConstant((void*)&nrect, sizeof(RD_Rect), 0, sync);
    pline->PartialPushConstant((void*)zero.GetData(), 4 * sizeof(float), 4 * sizeof(float), sync);
    pline->PartialPushConstant((void*)&id, sizeof(uint32_t), 8 * sizeof(float), sync);
    pline->DrawIndexedVertexBuffer(surface->GetVertexBuffer(), sync);
    pline->Unbind(sync);
}

void EXP_GuiButton::Event() {
    auto winsys = m_game->GetRenderer()->GetAPI()->GetWindowingSystem();
    auto guiman = m_game->GetGuiManager();
    vec2 curPos = vec2(winsys->GetCursorPositionX(true), winsys->GetCursorPositionY(true));

    switch(m_state) {
        case states_t::INIT:
            if(isPointInRect(curPos, m_rect)) {
                m_state = states_t::HOVER;
                guiman->SetRedrawFlag();
            }
            break;

        case states_t::HOVER:
            if(!isPointInRect(curPos, m_rect)) {
                m_state = states_t::INIT;
                guiman->SetRedrawFlag();
            }

            break;

        case states_t::CLICKED:
            break;
    }
}

void EXP_GuiButton::RegisterClick() {
    m_state = states_t::CLICKED;
    m_game->GetGuiManager()->SetRedrawFlag();
}

void EXP_GuiButton::UnregisterClick() {
    m_state = states_t::HOVER;
    m_game->GetGuiManager()->SetRedrawFlag();
}
