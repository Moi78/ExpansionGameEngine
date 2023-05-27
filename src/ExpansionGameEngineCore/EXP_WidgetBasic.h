#ifndef EXPGE_EXP_WIDGETBASIC_H
#define EXPGE_EXP_WIDGETBASIC_H

#include <memory>

#include "EXP_GuiManager.h"
#include "EXP_Material.h"
#include "EXP_Game.h"
#include "EXP_GenericRessourceManager.h"

class EXP_GuiSolidRect : public EXP_GuiWidget {
public:
    EXP_GuiSolidRect(EXP_Game* game, RD_Rect rect, vec4 color, std::weak_ptr<EXP_GuiWidget> parent = {});
    ~EXP_GuiSolidRect();

    void SetColor(vec4 nColor);

    void RenderWidget(std::shared_ptr<RD_Quad> surface, const RD_Rect& parentRect, std::shared_ptr<RD_RenderSynchronizer> sync) override;
    void Event() override {};

private:
    EXP_Game* m_game;
    std::shared_ptr<EXP_Material> m_mat;

    vec4 m_color;
};

class EXP_GuiSolidTexturedRect : public EXP_GuiWidget {
public:
    EXP_GuiSolidTexturedRect(EXP_Game* game, RD_Rect rect, std::string texPath, std::weak_ptr<EXP_GuiWidget> parent = {});
    ~EXP_GuiSolidTexturedRect();

    void RenderWidget(std::shared_ptr<RD_Quad> surface, const RD_Rect& parentRect, std::shared_ptr<RD_RenderSynchronizer> sync) override;
    void Event() override {};

private:
    std::shared_ptr<EXP_Material> m_mat;
    std::shared_ptr<RD_Texture> m_tex;
};

struct EXP_GlyphMetrics {
    vec2 bearing;
    vec2 size;
    int advance;
};

class EXP_Font {
public:
    EXP_Font(EXP_Game* game, FT_Library ft, std::string fontPath, bool isEngine);

    bool LoadFont();

    std::shared_ptr<RD_Texture> GetGlyphTex(char character, int size);
    EXP_GlyphMetrics GetGlyphMetrics(char character, int size);

    void SetCacheFlag(EXP_FontCacheFlags flags);
    void SetCacheSize(unsigned int size);
private:
    void CacheNumbers();
    void CacheLetters();

    EXP_Game* m_game;
    FT_Library m_ft;

    std::string m_fpath;
    FT_Face m_font;

    EXP_GenericRessourceManager<std::shared_ptr<RD_Texture>> m_tex_man;

    EXP_FontCacheFlags m_flags;
    unsigned int m_cached_size;
};

class EXP_GuiTextStatic : public EXP_GuiWidget {
public:
    EXP_GuiTextStatic(EXP_Game* game, RD_Rect rect, std::shared_ptr<EXP_Font> font, vec4 color, std::weak_ptr<EXP_GuiWidget> parent = {});
    ~EXP_GuiTextStatic();

    void ConstructText(std::string text, int size);

    void RenderWidget(std::shared_ptr<RD_Quad> surface, const RD_Rect& parentRect, std::shared_ptr<RD_RenderSynchronizer> sync) override;
    void Event() override {};

private:
    EXP_Game* m_game;

    std::shared_ptr<RD_API> m_api;
    std::shared_ptr<RD_RenderPass> m_text;

    std::shared_ptr<EXP_Material> m_mat;
    std::shared_ptr<EXP_Material> m_blit_mat;

    std::shared_ptr<EXP_Font> m_font;

    vec4 m_color;
};

class EXP_GuiEmpty : public EXP_GuiWidget {
public:
    EXP_GuiEmpty(RD_Rect rect, std::weak_ptr<EXP_GuiWidget> parent = {}) : EXP_GuiWidget(parent) {}
    ~EXP_GuiEmpty() = default;

    void RenderWidget(std::shared_ptr<RD_Quad> surface, const RD_Rect& parentRect, std::shared_ptr<RD_RenderSynchronizer> sync) override {}
    void Event() override {}
};

#endif //EXPGE_EXP_WIDGETBASIC_H
