#ifndef EXPGE_EXP_WIDGETBASIC_H
#define EXPGE_EXP_WIDGETBASIC_H

#include <memory>

#include "EXP_GuiManager.h"
#include "EXP_Material.h"
#include "EXP_Game.h"

class EXP_GuiSolidRect : public EXP_GuiWidget {
public:
    EXP_GuiSolidRect(EXP_Game* game, EXP_GuiWidget* parent = nullptr);
    ~EXP_GuiSolidRect();

    void RenderWidget(std::shared_ptr<RD_Quad> surface, std::shared_ptr<RD_RenderSynchronizer> sync) override;

private:
    std::shared_ptr<EXP_Material> m_mat;
};

#endif //EXPGE_EXP_WIDGETBASIC_H
