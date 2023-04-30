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

void EXP_GuiSolidRect::SetColor(vec4 nColor) {
    m_color = nColor;
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
                                                   std::weak_ptr<EXP_GuiWidget> parent) : EXP_GuiWidget(parent)
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

    constexpr float zeroData[4] = {1.0f, 1.0f, 1.0f, 1.0f};

    pline->Bind(sync);
    pline->PartialPushConstant((void*)&nrect, sizeof(RD_Rect), 0, sync);
    pline->PartialPushConstant((void*)zeroData, 4 * sizeof(float), 4 * sizeof(float), sync);
    pline->DrawIndexedVertexBuffer(surface->GetVertexBuffer(), sync);
    pline->Unbind(sync);
}

//----------------------------------------------------------------------------------------------------------------------
// EXP_Font

EXP_Font::EXP_Font(EXP_Game* game, FT_Library ft, std::string fontPath, bool isEngine) {
    std::string basePath = isEngine ? game->GetEngineContentPath() : game->GetGameContentPath();
    m_fpath = basePath + fontPath;

    m_game = game;
    m_ft = ft;
}

bool EXP_Font::LoadFont() {
    if(FT_New_Face(m_ft, m_fpath.c_str(), 0, &m_font)) {
        std::cerr << "ERROR: Failed to load font " << m_fpath << std::endl;
        return false;
    }

    return true;
}

std::shared_ptr<RD_Texture> EXP_Font::GetGlyphTex(char character, int size) {
    if(m_tex_man.DoIDExists(std::to_string(character) + std::to_string(size))) {
        return m_tex_man.GetRessource(std::to_string(character) + std::to_string(size));
    }

    FT_Set_Pixel_Sizes(m_font, 0, size);
    if(FT_Load_Char(m_font, character, FT_LOAD_RENDER)) {
        std::cerr << "ERROR: Failed to load character " << character << std::endl;

        auto tex = m_game->GetRenderer()->GetAPI()->CreateTexture();

        int data = 0;
        tex->CreateTextureFromData(IMGFORMAT_R, 1, 1, &data);

        m_tex_man.AddRessource(tex, std::to_string(character) + std::to_string(size));
        return tex;
    }

    auto tex = m_game->GetRenderer()->GetAPI()->CreateTexture();
    tex->CreateTextureFromData(IMGFORMAT_R, m_font->glyph->bitmap.width, m_font->glyph->bitmap.rows, m_font->glyph->bitmap.buffer);

    m_tex_man.AddRessource(tex, std::to_string(character) + std::to_string(size));
    return tex;
}

EXP_GlyphMetrics EXP_Font::GetGlyphMetrics(char character, int size) {
    FT_Set_Pixel_Sizes(m_font, 0, size);
    if(FT_Load_Char(m_font, character, FT_LOAD_BITMAP_METRICS_ONLY)) {
        std::cerr << "ERROR: Failed to get glyph " << character << " metrics..." << std::endl;
        return {};
    }

    EXP_GlyphMetrics met{};
    met.advance = (m_font->glyph->advance.x >> 6);
    met.size = vec2{(float)m_font->glyph->bitmap.width, (float)m_font->glyph->bitmap.rows};
    met.bearing = vec2{(float)m_font->glyph->bitmap_left, (float)m_font->glyph->bitmap_top};

    return met;
}

//----------------------------------------------------------------------------------------------------------------------
// EXP_GuiTextStatic

EXP_GuiTextStatic::EXP_GuiTextStatic(EXP_Game *game, RD_Rect rect, std::shared_ptr<EXP_Font> font,
                                     std::weak_ptr<EXP_GuiWidget> parent) : EXP_GuiWidget(parent) {
    m_api = game->GetRenderer()->GetAPI();
    m_font = font;
    m_game = game;

    m_mat = game->QueryMaterial("/ui/materials/glyph.json", true);
    m_blit_mat = game->QueryMaterial("/ui/materials/solid_tex.json", true);

    m_rect = rect;
}

EXP_GuiTextStatic::~EXP_GuiTextStatic() {

}

void EXP_GuiTextStatic::RenderWidget(std::shared_ptr<RD_Quad> surface, const RD_Rect &parentRect,
                                     std::shared_ptr<RD_RenderSynchronizer> sync) {
    auto pline = m_blit_mat->GetPipeline();

    RD_Rect nrect{m_rect};
    nrect.x += parentRect.x;
    nrect.y += parentRect.y;

    vec4 color{0.0f, 1.0f, 0.0f, 1.0f};

    pline->Bind(sync);

    pline->PartialPushConstant((void*)&nrect, sizeof(RD_Rect), 0, sync);
    pline->PartialPushConstant((void*)color.GetData(), 4 * sizeof(float), 4 * sizeof(float), sync);
    pline->DrawIndexedVertexBuffer(surface->GetVertexBuffer(), sync);

    pline->Unbind(sync);
}

void EXP_GuiTextStatic::ConstructText(std::string text, int size) {
    RD_Rect selfRect = m_rect;

    selfRect.h = size + (size / 2);
    selfRect.w = 0;
    for(auto c : text) {
        auto met = m_font->GetGlyphMetrics(c, size);

        selfRect.w += met.advance;
    }

    m_rect = selfRect;
    RD_Attachment monocolor{
            .format = IMGFORMAT_RGBA,
            .sample_count = 1,
            .do_clear = false,
            .clearColor = vec4(0.0f, 0.0f, 0.0f, 0.0f),
            .is_swapchain_attachment = false,
            .is_transparent = true
    };

    m_text = m_api->CreateRenderPass({monocolor}, selfRect.w, selfRect.h);
    m_text->BuildRenderpass(m_api.get(), false);

    auto pline = m_mat->GetPipeline();

    auto sync = m_api->CreateRenderSynchronizer();
    auto quad = std::make_shared<RD_Quad>(m_api);

    int currentX = 0;
    vec4 color{1.0f, 1.0f, 1.0f, 1.0f};

    for(auto c : text) {
        EXP_GlyphMetrics met = m_font->GetGlyphMetrics(c, size);

        if((met.size.GetX() == 0) || (met.size.GetY() == 0)) {
            currentX += met.advance;
            continue;
        }

        auto char_tex = m_font->GetGlyphTex(c, size);

        pline->PurgeTextures();
        pline->RegisterTexture(char_tex, 3);
        pline->RebuildPipeline();

        RD_Rect r{};
        r.w = met.size.GetX();
        r.h = met.size.GetY();

        r.x = currentX + met.bearing.GetX();
        r.y = size - met.bearing.GetY();

        sync->Start();
        m_text->BeginRenderpass(sync);
        pline->Bind(sync);

        pline->PartialPushConstant((void*)&r, sizeof(RD_Rect), 0, sync);
        pline->PartialPushConstant((void*)color.GetData(), 4 * sizeof(float), 4 * sizeof(float), sync);
        pline->DrawIndexedVertexBuffer(quad->GetVertexBuffer(), sync);

        pline->Unbind(sync);
        m_text->EndRenderpass(sync);
        sync->Stop();

        currentX += met.advance;
    }

    auto bpline = m_blit_mat->GetPipeline();
    bpline->RegisterTexture(m_text->GetAttachment(0), 3);
    bpline->RebuildPipeline();
}
