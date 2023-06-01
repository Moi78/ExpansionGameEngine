#ifndef EXPGE_EXP_GUIWINDOW_H
#define EXPGE_EXP_GUIWINDOW_H

#include <EXP_Game.h>
#include <EXP_GuiManager.h>
#include <EXP_WidgetBasic.h>
#include <EXP_MouseCallback.h>

enum EXP_WindowFlags {
    WIN_NONE = 0,
    WIN_DRAGABLE = 1,
    WIN_RESIZABLE = 2,
    WIN_NO_BAR = 4,
    WIN_NO_BORDERS = 8,
};

class EXP_GuiWindow : public EXP_GuiWidget {
public:
    EXP_GuiWindow(EXP_Game* game, RD_Rect pos, std::string title, EXP_WindowFlags flags, std::shared_ptr<EXP_GuiWidget> parent = {});
    ~EXP_GuiWindow();

    void SetBackgroundColor(vec4 color);
    void SetAccentColor(vec4 color);

    void RenderWidget(std::shared_ptr<RD_Quad> surface, const RD_Rect& parent_rect, std::shared_ptr<RD_RenderSynchronizer> sync) override;
    void Event() override;

private:
    void OnClick();
    void OnRelease();

    enum grab_state_t {RELEASED, GRABBED, CLICKED};

    EXP_Game* m_game;

    std::string m_title;
    vec4 m_bg_color, m_accent_color, m_border_color;
    unsigned int m_bar_height;

    std::shared_ptr<EXP_GuiSolidRect> m_bg_surface, m_bar_surface, m_border_surface;
    std::shared_ptr<EXP_GuiTextStatic> m_title_text;
    RD_Rect m_draggable_area;

    std::shared_ptr<EXP_MouseCallback> m_click;

    grab_state_t m_state;
    vec2 m_mouse_pos;

    EXP_WindowFlags m_flags;
};


#endif //EXPGE_EXP_GUIWINDOW_H
