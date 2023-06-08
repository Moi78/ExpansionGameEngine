#include "EXP_GuiManager.h"
#include "EXP_WidgetBasic.h"
#include "EXP_Game.h"

EXP_GuiWidget::EXP_GuiWidget(std::weak_ptr<EXP_GuiWidget> parent) {
    if(!parent.expired()) {
        m_parent = parent.lock();
        m_parent->AddChild(this);
    } else {
        m_parent = nullptr;
    }

    m_rect = {0, 0, 0, 0};
}

void EXP_GuiWidget::AddChild(EXP_GuiWidget *child) {
    m_child.push_back(child);
}

void EXP_GuiWidget::Paint(std::shared_ptr<RD_Quad> surface, const RD_Rect& parentRect, std::shared_ptr<RD_RenderSynchronizer> sync) {
    RenderWidget(surface, parentRect, sync);

    // To make the three follows the parent
    RD_Rect parented_rect{m_rect};
    parented_rect.x += parentRect.x;
    parented_rect.y += parentRect.y;

    for(auto& c : m_child) {
        c->Paint(surface, parented_rect, sync);
    }
}

void EXP_GuiWidget::ProcessEvents(vec2 curPos, bool leftButtonPress) {
    Event(curPos, leftButtonPress);

    for(auto& c : m_child) {
        c->ProcessEvents(curPos, leftButtonPress);
    }
}

const RD_Rect& EXP_GuiWidget::GetRect() {
    return m_rect;
}

//-------------------------------------------------------------------

EXP_GuiManager::EXP_GuiManager(EXP_Game* game, std::shared_ptr<RD_API> api) {
    m_api = api;
    m_winsys = api->GetWindowingSystem();

    m_mb_press = false;

    m_leftClick = std::make_shared<EXP_MouseCallback>(MOUSE_BUTTON_LEFT, CL_VDFUNCPTR(EXP_GuiManager::OnPressMB));
    m_leftClick->AttachOnKeyUp(CL_VDFUNCPTR(EXP_GuiManager::OnReleaseMB));
    game->GetInputHandler()->RegisterMouseCallback(m_leftClick);

    RD_Attachment color{};
    color.do_clear = true;
    color.format = IMGFORMAT_RGBA;
    color.is_swapchain_attachment = false;
    color.sample_count = 1;
    color.is_transparent = true;

    m_rpass = m_api->CreateRenderPass({color}, api->GetWindowingSystem()->GetWidth(), api->GetWindowingSystem()->GetHeight());
    m_rpass->BuildRenderpass(api.get(), false);

    m_sync = m_api->CreateRenderSynchronizer();

    m_surface = std::make_shared<RD_Quad>(api);
    m_redraw_flag = true;

    if(FT_Init_FreeType(&m_ft)) {
        std::cerr << "ERROR: Failed to init FreeType." << std::endl;
    } else {
        std::cout << "Init FreeType." << std::endl;
    }
}

void EXP_GuiManager::AddWidget(std::shared_ptr<EXP_GuiWidget> widget) {
    m_widgets.push_back(widget);
    m_redraw_flag = true;
}

std::shared_ptr<RD_RenderPass> EXP_GuiManager::GetRenderPass() {
    return m_rpass;
}

void EXP_GuiManager::RenderGui() {
    if(m_widgets.empty()) {
        return;
    }

    if(m_redraw_flag) {
        m_sync->Start();
        m_rpass->BeginRenderpass(m_sync);

        for (auto &w: m_widgets) {
            w->Paint(m_surface, {0, 0, 0, 0}, m_sync);
        }

        m_rpass->EndRenderpass(m_sync);
        m_sync->Stop();

        m_redraw_flag = false;
    }
}

void EXP_GuiManager::Resize(int w, int h) {
    m_rpass->SetRenderpassSize(m_api.get(), w, h);
    SetRedrawFlag();
}

void EXP_GuiManager::ProcessEvents() {
    vec2 curPos = vec2(m_winsys->GetCursorPositionX(true), m_winsys->GetCursorPositionY(true));

    for(auto& w : m_widgets) {
        w->ProcessEvents(curPos, m_mb_press);
    }
}

std::shared_ptr<EXP_Font> EXP_GuiManager::ConstructFont(EXP_Game* game, std::string fontPath, bool isEngine, EXP_FontCacheFlags flag) {
    auto f = std::make_shared<EXP_Font>(game, m_ft, fontPath, isEngine);
    f->SetCacheFlag(flag);

    return f;
}

void EXP_GuiManager::SetRedrawFlag() {
    m_redraw_flag = true;
}

void EXP_GuiManager::OnPressMB() {
    m_mb_press = true;
}

void EXP_GuiManager::OnReleaseMB() {
    m_mb_press = false;
}

//----------------------------------------------------------------------------------------------------------------------

bool isPointInRect(vec2& point, RD_Rect& r) {
    const float pX = point.GetX();
    const float pY = point.GetY();

    return (pX > r.x) && (pX < (r.x + r.w)) && (pY > r.y) && (pY < r.y + r.h);
}
