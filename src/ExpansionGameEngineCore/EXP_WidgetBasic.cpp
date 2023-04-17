#include "EXP_WidgetBasic.h"

// EXP_GuiSolidRect

EXP_GuiSolidRect::EXP_GuiSolidRect(EXP_Game* game, RD_Rect rect, vec4 color, EXP_GuiWidget* parent) :
    EXP_GuiWidget(parent),
    m_color(color),
    m_rect(rect)
{
    m_mat = game->QueryMaterial("/ui/materials/solid.json", true);
}

EXP_GuiSolidRect::~EXP_GuiSolidRect() {

}

void EXP_GuiSolidRect::RenderWidget(std::shared_ptr<RD_Quad> surface, std::shared_ptr<RD_RenderSynchronizer> sync) {
    auto pline = m_mat->GetPipeline();

    pline->Bind(sync);
    pline->PartialPushConstant((void*)&m_rect, sizeof(RD_Rect), 0, sync);
    pline->PartialPushConstant((void*)m_color.GetData(), 4 * sizeof(float), 4 * sizeof(float), sync);
    pline->DrawIndexedVertexBuffer(surface->GetVertexBuffer(), sync);
    pline->Unbind(sync);
}