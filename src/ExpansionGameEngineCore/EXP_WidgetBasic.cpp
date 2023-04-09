#include "EXP_WidgetBasic.h"

// EXP_GuiSolidRect

EXP_GuiSolidRect::EXP_GuiSolidRect(EXP_Game* game, EXP_GuiWidget* parent) : EXP_GuiWidget(parent) {
    m_mat = game->QueryMaterial("/ui/materials/solid.json", true);
}

EXP_GuiSolidRect::~EXP_GuiSolidRect() {

}

void EXP_GuiSolidRect::RenderWidget(std::shared_ptr<RD_Quad> surface, std::shared_ptr<RD_RenderSynchronizer> sync) {
    auto pline = m_mat->GetPipeline();

    const RD_Rect r = {
            .x = 0.0f,
            .y = 0.0f,
            .w = 0.5f,
            .h = 0.5f
    };

    pline->Bind(sync);
    pline->PushConstant((void*)&r, sizeof(RD_Rect), sync);
    pline->DrawIndexedVertexBuffer(surface->GetVertexBuffer(), sync);
    pline->Unbind(sync);
}