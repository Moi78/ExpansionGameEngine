#include "EXP_GuiWindow.h"

EXP_GuiWindow::EXP_GuiWindow(EXP_Game *game, RD_Rect pos, std::string title, EXP_WindowFlags flags,
                             std::shared_ptr<EXP_GuiWidget> parent) :
                             EXP_GuiWidget(parent),
                             m_game(game), m_title(title), m_flags(flags), m_bar_height(30),
                             m_bg_color(COLOR_HEX_FLOAT(0x20, 0x4E, 0x4A, 0xFF)),
                             m_accent_color(COLOR_HEX_FLOAT(0x29, 0x70, 0x45, 0xFF)),
                             m_border_color(COLOR_HEX_FLOAT(0x0A, 0x17, 0x16, 0xFF))
{
    m_rect = pos;
    m_draggable_area = RD_Rect{0, 0, pos.w, pos.h};

    m_state = grab_state_t::RELEASED;

    if(flags & EXP_WindowFlags::WIN_DRAGABLE) {
        m_click = std::make_shared<EXP_MouseCallback>(GLFW_MOUSE_BUTTON_LEFT, CL_VDFUNCPTR(EXP_GuiWindow::OnClick), false);
        m_click->AttachOnKeyUp(CL_VDFUNCPTR(EXP_GuiWindow::OnRelease));

        m_game->GetInputHandler()->RegisterMouseCallback(m_click);
    }

    // Initalizing window surfaces
    m_bg_surface = std::make_shared<EXP_GuiSolidRect>(game, RD_Rect{0, 0, pos.w, pos.h}, m_bg_color);
    AddChild(m_bg_surface.get());

    if(!(flags & EXP_WindowFlags::WIN_NO_BAR)) {
        RD_Rect bar_pos = {
                .x = 0,
                .y = -(float)m_bar_height,
                .w = pos.w,
                .h = (float)m_bar_height
        };

        RD_Rect title_pos = RD_Rect(bar_pos);
        title_pos.y += m_bar_height / 4;
        title_pos.x += 10;

        m_draggable_area = bar_pos;

        m_bar_surface = std::make_shared<EXP_GuiSolidRect>(game, bar_pos, m_accent_color);

        m_title_text = std::make_shared<EXP_GuiTextStatic>(game, title_pos, game->GetEngineFont(), vec4(1.0f, 1.0f, 1.0f, 1.0f));
        m_title_text->ConstructText(title, m_bar_height / 2);

        AddChild(m_bar_surface.get());
        AddChild(m_title_text.get());
    }

    if(!(flags & EXP_WindowFlags::WIN_NO_BORDERS)) {
        RD_Rect fill_bg{pos};
        fill_bg.x = 1;
        fill_bg.y = 1;
        fill_bg.w -= 2;
        fill_bg.h -= 2;

        m_border_surface = std::make_shared<EXP_GuiSolidRect>(game, fill_bg, m_bg_color);
        m_bg_surface->SetColor(m_border_color);

        AddChild(m_border_surface.get());
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
}

void EXP_GuiWindow::Event() {
    auto winsys = m_game->GetRenderer()->GetAPI()->GetWindowingSystem();
    vec2 mousepos = vec2(winsys->GetCursorPositionX(true), winsys->GetCursorPositionY(true));
    vec2 mousepos_winspace = mousepos - vec2(m_rect.x, m_rect.y);

    vec2 delta;
    switch(m_state) {
        case grab_state_t::GRABBED:
            delta = mousepos - m_mouse_pos;
            m_rect.x += delta.GetX();
            m_rect.y += delta.GetY();

            m_game->GetGuiManager()->SetRedrawFlag();

            break;
        case grab_state_t::RELEASED:
            break;
        case grab_state_t::CLICKED:
            if(isPointInRect(mousepos_winspace, m_draggable_area)) {
                m_state = grab_state_t::GRABBED;
            }

            break;
    }

    m_mouse_pos = mousepos;
}

void EXP_GuiWindow::OnClick() {
    m_state = grab_state_t::CLICKED;
}

void EXP_GuiWindow::OnRelease() {
    m_state = grab_state_t::RELEASED;
}
