#ifndef EXPGE_EXP_GUIMANAGER_H
#define EXPGE_EXP_GUIMANAGER_H

#include <RD_API.h>
#include <RD_Quad.h>

#include <memory>
#include <vector>

class EXP_GuiWidget {
public:
    EXP_GuiWidget(std::weak_ptr<EXP_GuiWidget> parent = {});
    ~EXP_GuiWidget() = default;

    virtual void RenderWidget(std::shared_ptr<RD_Quad> surface, const RD_Rect& parentRect, std::shared_ptr<RD_RenderSynchronizer> sync) = 0;
    virtual void Paint(std::shared_ptr<RD_Quad> surface, const RD_Rect& parentRect, std::shared_ptr<RD_RenderSynchronizer> sync);

    void ProcessEvents();
    virtual void Event() = 0;

    const RD_Rect& GetRect();

protected:
    void AddChild(EXP_GuiWidget* child);

    std::shared_ptr<EXP_GuiWidget> m_parent;
    std::vector<EXP_GuiWidget*> m_child;

    RD_Rect m_rect;
};

class EXP_GuiManager {
public:
    EXP_GuiManager(std::shared_ptr<RD_API> api);
    ~EXP_GuiManager() = default;

    std::shared_ptr<RD_RenderPass> GetRenderPass();

    void AddWidget(std::shared_ptr<EXP_GuiWidget> widget);

    void RenderGui();
    void Resize(int w, int h);

    void ProcessEvents();

private:
    std::shared_ptr<RD_API> m_api;

    std::shared_ptr<RD_RenderPass> m_rpass;
    std::shared_ptr<RD_RenderSynchronizer> m_sync;
    std::shared_ptr<RD_Quad> m_surface;

    std::vector<std::shared_ptr<EXP_GuiWidget>> m_widgets;
};


#endif //EXPGE_EXP_GUIMANAGER_H
