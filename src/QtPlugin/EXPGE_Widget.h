#ifndef EXPGE_EXPGE_WIDGET_H
#define EXPGE_EXPGE_WIDGET_H

#include <EXP_Game.h>
#include <EXP_Animator.h>
#include <EXP_Level.h>

#include <RaindropRenderer.h>
#include <RD_API.h>

#include <QWidget>

#include <QWindow>
#include <QVulkanWindow>

class VulkanWindow : public QVulkanWindow {
public:
    QVulkanWindowRenderer* createRenderer() override;

    void setEXPGEWin(RD_VulkanWindow* expgewin);

private:
    RD_VulkanWindow* m_expgewin;
};

class VulkanRenderer : public QVulkanWindowRenderer {
public:
    VulkanRenderer(RD_VulkanWindow* expgewin, QVulkanWindow* qwin);

    void startNextFrame() override;

private:
    QVulkanWindow* m_win;

    RD_VulkanWindow* m_expgewin;
    RD_API* m_api;
};

class EXPGE_Widget : public QWidget, public RD_VulkanWindow {

    Q_OBJECT

public:
    EXPGE_Widget(std::shared_ptr<RD_API> api, QWidget* parent = nullptr);
    ~EXPGE_Widget();

    bool OpenWindow(std::string name, const int w, const int h) override;
    bool WantToClose() override;

    void PollEvents() override;

    void SetCursorVisibility(bool visibility) override;

    float GetCursorPositionX(bool abs = false) override;
    float GetCursorPositionY(bool abs = false) override;
    void SetCursorPosition(double x, double y) override;

    bool GetKeyPress(int key) override;
    bool GetMouseButtonPress(int mbutton) override;

    int GetScreenHeight() override;
    int GetScreenWidth() override;

    int GetExtensionsCount() override;
    const char** GetExtensionsNames() override;

    VkResult CreateWindowSurface(VkInstance inst) override;

    std::vector<const char*> GetRequiredExtensions(bool validationEnable) override;

private:
    QVulkanInstance* m_qinst;

    VulkanWindow* m_win;
    VulkanRenderer* m_rndr;

    QWidget* m_win_container;
};

class QEXPGE : public QWidget {

    Q_OBJECT

public:
    QEXPGE(QWidget* parent = nullptr);
    ~QEXPGE();

    bool InitEngine();

    void paintEvent(QPaintEvent* e);

private:
    std::shared_ptr<RD_API> m_api;
    std::shared_ptr<RD_RenderingPipeline> m_rpline;
    std::shared_ptr<RaindropRenderer> m_rndr;
    std::unique_ptr<EXP_Game> m_game;

    std::shared_ptr<EXPGE_Widget> m_winsys;
};


#endif //EXPGE_EXPGE_WIDGET_H
