#include "EXP_GuiWindow.h"

EXP_GuiWindow::EXP_GuiWindow(EXP_Game *game, RD_Rect pos, std::string title, EXP_WindowFlags flags,
                             std::shared_ptr<EXP_GuiWidget> parent) :
                             EXP_GuiWidget(parent),
                             m_game(game), m_title(title), m_flags(flags), m_bar_height(30),
                             m_bg_color(COLOR_HEX_FLOAT(0x20, 0x4E, 0x4A, 0xFF)),
                             m_accent_color(COLOR_HEX_FLOAT(0x29, 0x70, 0x45, 0xFF))
{
    // Initalizing window surfaces
    m_bg_surface = std::make_shared<EXP_GuiSolidRect>(game, pos, m_bg_color);
    if(!(flags & EXP_WindowFlags::WIN_NO_BAR)) {
        RD_Rect bar_pos = {
                .x = pos.x,
                .y = pos.y - m_bar_height,
                .w = pos.w,
                .h = (float)m_bar_height
        };

        RD_Rect title_pos = RD_Rect(bar_pos);
        title_pos.y += m_bar_height / 4;
        title_pos.x += 10;

        m_bar_surface = std::make_shared<EXP_GuiSolidRect>(game, bar_pos, m_accent_color);

        m_title_text = std::make_shared<EXP_GuiTextStatic>(game, title_pos, game->GetEngineFont(), vec4(1.0f, 1.0f, 1.0f, 1.0f));
        m_title_text->ConstructText(title, m_bar_height / 2);
    }
}

EXP_GuiWindow::~EXP_GuiWindow() {

}

void EXP_GuiWindow::SetBackgroundColor(vec4 color) {
    m_bg_color = color;
    m_bg_surface->SetColor(color);
}

void EXP_GuiWindow::SetAccentColor(vec4 color) {
    m_accent_color = color;

    if(m_bar_surface.use_count()) {
        m_bar_surface->SetColor(color);
    }
}

void EXP_GuiWindow::RenderWidget(std::shared_ptr<RD_Quad> surface, const RD_Rect &parent_rect,
                                 std::shared_ptr<RD_RenderSynchronizer> sync) {
    if(m_bar_surface.use_count()) {
        m_bar_surface->Paint(surface, parent_rect, sync);
        m_title_text->Paint(surface, parent_rect, sync);
    }

    m_bg_surface->Paint(surface, parent_rect, sync);
}

void EXP_GuiWindow::Event() {

}
