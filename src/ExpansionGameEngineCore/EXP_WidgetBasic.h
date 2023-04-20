#ifndef EXPGE_EXP_WIDGETBASIC_H
#define EXPGE_EXP_WIDGETBASIC_H

#include <memory>

#include "EXP_GuiManager.h"
#include "EXP_Material.h"
#include "EXP_Game.h"

class EXP_GuiSolidRect : public EXP_GuiWidget {
public:
    EXP_GuiSolidRect(EXP_Game* game, RD_Rect rect, vec4 color , std::weak_ptr<EXP_GuiWidget> parent = {});
    ~EXP_GuiSolidRect();

    void RenderWidget(std::shared_ptr<RD_Quad> surface, const RD_Rect& parentRect, std::shared_ptr<RD_RenderSynchronizer> sync) override;
    void Event() override {};

private:
    std::shared_ptr<EXP_Material> m_mat;

    vec4 m_color;
};

#endif //EXPGE_EXP_WIDGETBASIC_H
