#ifndef EXPGE_EXP_GUIMANAGER_H
#define EXPGE_EXP_GUIMANAGER_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include <RD_API.h>
#include <RD_Quad.h>

#include <EXP_MouseCallback.h>

#include <memory>
#include <vector>

class EXP_GuiWidget {
public:
    EXP_GuiWidget(std::weak_ptr<EXP_GuiWidget> parent = {});
    virtual ~EXP_GuiWidget() = default;

    virtual void RenderWidget(std::shared_ptr<RD_Quad> surface, const RD_Rect& parentRect, std::shared_ptr<RD_RenderSynchronizer> sync) = 0;
    virtual void Paint(std::shared_ptr<RD_Quad> surface, const RD_Rect& parentRect, std::shared_ptr<RD_RenderSynchronizer> sync);

    void ProcessEvents(vec2 curPos, bool leftButtonPress);
    virtual void Event(vec2 curPos, bool leftButtonPress) = 0;

    const RD_Rect& GetRect();

protected:
    void AddChild(EXP_GuiWidget* child);

    std::shared_ptr<EXP_GuiWidget> m_parent;
    std::vector<EXP_GuiWidget*> m_child;

    RD_Rect m_rect;
};

class EXP_Font;
class EXP_Game;

enum EXP_FontCacheFlags {
    OVER_TIME,
    ALL_LETTERS,
    ALL_NUMBERS,
    ALL_ALPHANUM
};

class EXP_GuiManager {
public:
    EXP_GuiManager(EXP_Game* game, std::shared_ptr<RD_API> api);
    ~EXP_GuiManager() = default;

    std::shared_ptr<RD_RenderPass> GetRenderPass();

    void AddWidget(std::shared_ptr<EXP_GuiWidget> widget);

    void RenderGui();
    void Resize(int w, int h);
    void SetRedrawFlag();

    void ProcessEvents();

    std::shared_ptr<EXP_Font> ConstructFont(EXP_Game* game, std::string fontPath, bool isEngine = false, EXP_FontCacheFlags cache_flag = OVER_TIME);

private:
    void OnPressMB();
    void OnReleaseMB();

    std::shared_ptr<RD_API> m_api;
    std::shared_ptr<RD_Windowing> m_winsys;

    std::shared_ptr<RD_RenderPass> m_rpass;
    std::shared_ptr<RD_RenderSynchronizer> m_sync;
    std::shared_ptr<RD_Quad> m_surface;

    std::vector<std::shared_ptr<EXP_GuiWidget>> m_widgets;

    std::shared_ptr<EXP_MouseCallback> m_leftClick;
    bool m_mb_press;

    FT_Library m_ft;

    bool m_redraw_flag;
};

bool isPointInRect(vec2& point, RD_Rect& r);

#endif //EXPGE_EXP_GUIMANAGER_H
