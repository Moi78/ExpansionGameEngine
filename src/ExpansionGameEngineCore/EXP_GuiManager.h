#ifndef EXPGE_EXP_GUIMANAGER_H
#define EXPGE_EXP_GUIMANAGER_H

#include <RD_API.h>
#include <RD_OrphanFramebuffer.h>

#include <memory>
#include <vector>

class EXP_GuiWidget {
public:
    EXP_GuiWidget(EXP_GuiWidget* parent = nullptr);
    ~EXP_GuiWidget() = default;

    virtual void RenderWidget() = 0;
    void Paint();

    const RD_Rect& GetRect();

protected:
    void AddChild(EXP_GuiWidget* child);

    EXP_GuiWidget* m_parent;
    std::vector<EXP_GuiWidget*> m_child;

    RD_Rect m_rect;
};

class EXP_GuiManager {
public:
    EXP_GuiManager(std::shared_ptr<RD_API> api);
    ~EXP_GuiManager() = default;

    void AddWidget(std::shared_ptr<EXP_GuiWidget> widget);

    void RenderGui();

private:
    std::shared_ptr<RD_API> m_api;

    std::vector<std::shared_ptr<EXP_GuiWidget>> m_widgets;
};


#endif //EXPGE_EXP_GUIMANAGER_H
