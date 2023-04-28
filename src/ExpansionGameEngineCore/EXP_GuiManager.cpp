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

    for(auto& c : m_child) {
        c->Paint(surface, m_rect, sync);
    }
}

void EXP_GuiWidget::ProcessEvents() {
    Event();

    for(auto& c : m_child) {
        c->ProcessEvents();
    }
}

const RD_Rect& EXP_GuiWidget::GetRect() {
    return m_rect;
}

//-------------------------------------------------------------------

EXP_GuiManager::EXP_GuiManager(std::shared_ptr<RD_API> api) {
    m_api = api;

    RD_Attachment color{};
    color.do_clear = true;
    color.format = IMGFORMAT_RGBA;
    color.is_swapchain_attachment = false;
    color.sample_count = 1;

    m_rpass = m_api->CreateRenderPass({color}, api->GetWindowingSystem()->GetWidth(), api->GetWindowingSystem()->GetHeight());
    m_rpass->BuildRenderpass(api.get(), false);

    m_sync = m_api->CreateRenderSynchronizer();

    m_surface = std::make_shared<RD_Quad>(api);

    if(FT_Init_FreeType(&m_ft)) {
        std::cerr << "ERROR: Failed to init FreeType." << std::endl;
    } else {
        std::cout << "Init FreeType." << std::endl;
    }
}

void EXP_GuiManager::AddWidget(std::shared_ptr<EXP_GuiWidget> widget) {
    m_widgets.push_back(widget);
}

std::shared_ptr<RD_RenderPass> EXP_GuiManager::GetRenderPass() {
    return m_rpass;
}

void EXP_GuiManager::RenderGui() {
    if(m_widgets.empty()) {
        return;
    }

    m_sync->Start();
    m_rpass->BeginRenderpass(m_sync);

    for(auto& w : m_widgets) {
        w->Paint(m_surface, {0, 0, 0, 0}, m_sync);
    }

    m_rpass->EndRenderpass(m_sync);
    m_sync->Stop();
}

void EXP_GuiManager::Resize(int w, int h) {
    m_rpass->SetRenderpassSize(m_api.get(), w, h);
}

void EXP_GuiManager::ProcessEvents() {
    for(auto& w : m_widgets) {
        w->ProcessEvents();
    }
}

std::shared_ptr<EXP_Font> EXP_GuiManager::ConstructFont(EXP_Game* game, std::string fontPath, bool isEngine) {
    return std::make_shared<EXP_Font>(game, m_ft, fontPath, isEngine);
}
