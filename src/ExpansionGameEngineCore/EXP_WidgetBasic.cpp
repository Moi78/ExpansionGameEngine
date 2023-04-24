#include "EXP_WidgetBasic.h"

// EXP_GuiSolidRect

EXP_GuiSolidRect::EXP_GuiSolidRect(EXP_Game* game, RD_Rect rect, vec4 color, std::weak_ptr<EXP_GuiWidget> parent) :
    EXP_GuiWidget(parent),
    m_color(color)
{
    m_mat = game->QueryMaterial("/ui/materials/solid.json", true);
    m_rect = rect;
}

EXP_GuiSolidRect::~EXP_GuiSolidRect() {

}

void EXP_GuiSolidRect::RenderWidget(std::shared_ptr<RD_Quad> surface, const RD_Rect& parentRect, std::shared_ptr<RD_RenderSynchronizer> sync) {
    auto pline = m_mat->GetPipeline();

    RD_Rect nrect{m_rect};
    nrect.x += parentRect.x;
    nrect.y += parentRect.y;

    pline->Bind(sync);
    pline->PartialPushConstant((void*)&nrect, sizeof(RD_Rect), 0, sync);
    pline->PartialPushConstant((void*)m_color.GetData(), 4 * sizeof(float), 4 * sizeof(float), sync);
    pline->DrawIndexedVertexBuffer(surface->GetVertexBuffer(), sync);
    pline->Unbind(sync);
}

//----------------------------------------------------------------------------------------------------------------------
// EXP_GuiSolidTexturedRect

EXP_GuiSolidTexturedRect::EXP_GuiSolidTexturedRect(EXP_Game *game, RD_Rect rect, std::string texPath,
                                                   std::weak_ptr<EXP_GuiWidget> parent) :
        EXP_GuiWidget(parent)
{
    m_rect = rect;
    m_tex = game->GetRenderer()->GetAPI()->CreateTexture();

    std::string gamePath = game->GetGameContentPath();
    if(m_tex->LoadTextureFromFile(gamePath + texPath)) {
        m_mat = game->QueryMaterial("/ui/materials/solid_tex.json", true);
        m_mat->GetPipeline()->RegisterTexture(m_tex, 3);
        m_mat->GetPipeline()->RebuildPipeline();
    } else {
        m_mat = game->QueryMaterial("/ui/materials/solid.json", true);
    }
}

EXP_GuiSolidTexturedRect::~EXP_GuiSolidTexturedRect() {

}

void EXP_GuiSolidTexturedRect::RenderWidget(std::shared_ptr<RD_Quad> surface, const RD_Rect &parentRect,
                                            std::shared_ptr<RD_RenderSynchronizer> sync) {
    auto pline = m_mat->GetPipeline();

    RD_Rect nrect{m_rect};
    nrect.x += parentRect.x;
    nrect.y += parentRect.y;

    constexpr float zeroData[4] = {0.0f, 0.0f, 0.0f, 0.0f};

    pline->Bind(sync);
    pline->PartialPushConstant((void*)&nrect, sizeof(RD_Rect), 0, sync);
    pline->PartialPushConstant((void*)zeroData, 4 * sizeof(float), 4 * sizeof(float), sync);
    pline->DrawIndexedVertexBuffer(surface->GetVertexBuffer(), sync);
    pline->Unbind(sync);
}
