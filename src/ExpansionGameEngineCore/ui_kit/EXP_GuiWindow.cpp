#include "EXP_GuiWindow.h"

EXP_GuiWindow::EXP_GuiWindow(EXP_Game *game, RD_Rect pos, std::string title, EXP_WindowFlags flags,
                             std::shared_ptr<EXP_GuiWidget> parent) :
                             EXP_GuiWidget(parent),
                             m_game(game), m_title(title), m_flags(flags), m_bar_height(30),
                             m_bg_color(COLOR_HEX_FLOAT(0x58, 0x63, 0xF8, 0xFF)), m_accent_color(0.03f, 0.16f, 0.27f, 1.0f)
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

        m_bar_surface = std::make_shared<EXP_GuiSolidRect>(game, bar_pos, m_accent_color);
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
    }

    m_bg_surface->Paint(surface, parent_rect, sync);
}

void EXP_GuiWindow::Event() {

}
