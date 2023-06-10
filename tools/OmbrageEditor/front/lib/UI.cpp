#include "UI.h"

Ombrage::UI::UI(EXP_Game *game) {
    m_game = game;

    m_light_control = std::make_shared<EXP_GuiWindow>(game, RD_Rect{0, 390, 401, 100}, "Light Control", EXP_WindowFlags::WIN_NONE);
    m_file_control = std::make_shared<EXP_GuiWindow>(game, RD_Rect{0, 520, 401, 200}, "File", EXP_WindowFlags::WIN_NONE);
    m_work_area = std::make_shared<EXP_GuiWindow>(game, RD_Rect{400, 30, 880, 690}, "Node Tree", EXP_WindowFlags::WIN_NONE);

    m_save_btn = std::make_shared<EXP_GuiButton>(game, RD_Rect{10, 10, 380, 30}, "", "/texs/normal.png", "/texs/normal.jpg", m_file_control);

    game->GetRenderer()->GetAPI()->GetWindowingSystem()->SetViewport(RD_Rect{0, 0, 400, 400});
    game->GetRenderer()->GetAPI()->GetWindowingSystem()->SetViewportMode(RD_ViewportMode::FLOATING);
}

Ombrage::UI::~UI() {

}

void Ombrage::UI::ContructUI() {
    m_game->GetGuiManager()->AddWidget(m_light_control);
    m_game->GetGuiManager()->AddWidget(m_file_control);
    m_game->GetGuiManager()->AddWidget(m_work_area);
}
