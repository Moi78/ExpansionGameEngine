#include "EXPGE_Widget.h"

void VulkanWindow::setEXPGEWin(RD_VulkanWindow* expgewin) {
    m_expgewin = expgewin;
}

QVulkanWindowRenderer *VulkanWindow::createRenderer() {
    return new VulkanRenderer(m_expgewin ,this);
}

// -------------------------------------------------------------------------

VulkanRenderer::VulkanRenderer(RD_VulkanWindow* expgewin, QVulkanWindow *w) {
    m_win = w;
    m_expgewin = expgewin;
}

void VulkanRenderer::startNextFrame() {
    m_expgewin->Present();
}

// -------------------------------------------------------------------------

EXPGE_Widget::EXPGE_Widget(std::shared_ptr<RD_API> api, QWidget *parent) : RD_VulkanWindow(api) {
    m_qinst = new QVulkanInstance();
    m_win = new VulkanWindow();
    m_win->setEXPGEWin(this);
}

EXPGE_Widget::~EXPGE_Widget() {

}

bool EXPGE_Widget::OpenWindow(std::string name, const int w, const int h) {
    m_win->setVulkanInstance(m_qinst);
    m_win->setObjectName(QString::fromStdString(name));
    m_win_container = QWidget::createWindowContainer(m_win, this);

    m_win_container->show();

    if(!m_win_container & false) {
        return false;
    }

    return true;
}

bool EXPGE_Widget::WantToClose() {
    return false;
}

void EXPGE_Widget::PollEvents() {

}

void EXPGE_Widget::SetCursorVisibility(bool visibility) {

}

float EXPGE_Widget::GetCursorPositionX(bool abs) {
    return 0;
}

float EXPGE_Widget::GetCursorPositionY(bool abs) {
    return 0;
}

void EXPGE_Widget::SetCursorPosition(double x, double y) {

}

bool EXPGE_Widget::GetKeyPress(int key) {
    return false;
}

bool EXPGE_Widget::GetMouseButtonPress(int mbutton) {
    return false;
}

int EXPGE_Widget::GetScreenHeight() {
    return QWidget::height();
}

int EXPGE_Widget::GetScreenWidth() {
    return QWidget::width();
}

int EXPGE_Widget::GetExtensionsCount() {
    return m_qinst->extensions().size();
}

const char** EXPGE_Widget::GetExtensionsNames() {
    QByteArrayList qnames = m_qinst->extensions();
    static std::vector<const char*> names;

    for(auto n : qnames) {
        names.push_back(n.toStdString().c_str());
    }

    return names.data();
}

VkResult EXPGE_Widget::CreateWindowSurface(VkInstance inst) {
    m_qinst->setVkInstance(inst);
    if(!m_qinst->create()) {
        return m_qinst->errorCode();
    }

    m_surface = m_qinst->surfaceForWindow(m_win);

    if(!m_surface) {
        return VK_ERROR_UNKNOWN;
    }

    return VK_SUCCESS;
}

std::vector<const char*> EXPGE_Widget::GetRequiredExtensions(bool validationEnable) {
    std::vector<const char*> reqExt{};

    const char** extList = GetExtensionsNames();
    for(int i = 0; i < GetExtensionsCount(); i++) {
        reqExt.push_back(extList[i]);
    }

    reqExt.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

#ifdef __linux__
    reqExt.push_back("VK_KHR_xcb_surface");
#elif WIN32
    reqExt.push_back("VK_KHR_win32_surface");
#endif

    if(validationEnable) {
        reqExt.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return reqExt;
}

// -------------------------------------------------------------------------------------------

QEXPGE::QEXPGE(QWidget* parent) : QWidget(parent) {

}

QEXPGE::~QEXPGE() {

}

bool QEXPGE::InitEngine() {
    EXP_GameInfo gameinfo{};
    gameinfo.GameLib = "./libGame.so";
    gameinfo.RootEngineContentDir = "./Engine/";
    gameinfo.RootGameContentDir = "./Content/";
    gameinfo.WindowHeight = 720;
    gameinfo.WindowWidth = 1280;
    gameinfo.StartLevel = "/levels/level1.json";

    m_api = std::make_shared<RD_API_Vk>();
    m_rpline = std::make_shared<RD_RenderingPipeline_PBR>(m_api);

    m_winsys = std::make_shared<EXPGE_Widget>(m_api, this);
    m_api->SetWindowSystem(m_winsys);

    m_rndr = std::make_shared<RaindropRenderer>(std::shared_ptr<RD_API>(m_api), std::shared_ptr<RD_RenderingPipeline>(m_rpline), gameinfo.WindowWidth, gameinfo.WindowHeight, "ExGame");

    std::unique_ptr<EXP_Game> game = std::make_unique<EXP_Game>(m_rndr, gameinfo);

    if(!game->InitEngine()) {
        std::cerr << "ERROR: Failed to init engine. :(" << std::endl;
        return false;
    }

    return true;
}

void QEXPGE::paintEvent(QPaintEvent *e) {
    auto glayer = m_game->GetGuiManager();
    auto inhdl = m_game->GetInputHandler();

    inhdl->ResetCursor();

    m_rndr->UpdateWindow();
    glayer->RenderGui();
    m_rndr->RenderScene();
    m_rndr->Present();

    std::thread anim([this]() {
        m_game->GetAnimator()->UpdateAnimations();
    });

    std::thread logic([this, &inhdl, &glayer]() {
        inhdl->UpdateAll();
        glayer->ProcessEvents();

        m_game->GetCurrentLevel()->TickActors();
        m_game->GetCurrentLevel()->OnTick();
    });

    anim.join();
    logic.join();
}
