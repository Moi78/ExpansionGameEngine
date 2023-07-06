#include "UI.h"
#include "MathNodes.h"

OmbrageUI::UI::UI(EXP_Game *game) {
    m_game = game;

    m_oldSize = {0, 0};
    m_oldPos = {0, 0};

    m_node_editor = std::make_unique<NodeGraph_UI>();

    auto rootNode = std::make_shared<ShaderNode>(0);
    m_node_editor->AddNode(rootNode);

    m_node_editor->AddNodeToCatalog("Constants", "Const Float", [this](int id) { return std::make_shared<OmbrageNodes::ConstFloatNode>(id); });
    m_node_editor->AddNodeToCatalog("Constants", "Const Vec2", [this](int id) { return std::make_shared<OmbrageNodes::ConstVec2Node>(id); });
    m_node_editor->AddNodeToCatalog("Constants", "Const Vec3", [this](int id) { return std::make_shared<OmbrageNodes::ConstVec3Node>(id); });
    m_node_editor->AddNodeToCatalog("Constants", "Const Vec4", [this](int id) { return std::make_shared<OmbrageNodes::ConstVec4Node>(id); });

    m_node_editor->AddNodeToCatalog("Basic Maths", "Add", [this](int id) { return std::make_shared<OmbrageNodes::AddNode>(id); });
    m_node_editor->AddNodeToCatalog("Basic Maths", "Multiply", [this](int id) { return std::make_shared<OmbrageNodes::MultNode>(id); });
}

OmbrageUI::UI::~UI() {

}

void OmbrageUI::UI::SetupViewport() {
    auto winsys = m_game->GetRenderer()->GetAPI()->GetWindowingSystem();

    winsys->SetViewport({0, 0, 400, 400});
    winsys->SetViewportMode(RD_ViewportMode::FLOATING);
}

void OmbrageUI::UI::RenderImGui() {
    static auto winsys = m_game->GetRenderer()->GetAPI()->GetWindowingSystem();

    DockSpaceHandling();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4{0.0, 0.0, 0.0, 0.0});

    ImGui::SetNextWindowSizeConstraints({400, 400}, {100000, 100000});
    ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoCollapse);

    ImVec2 sz = ImGui::GetWindowSize();
    ImVec2 pos = ImGui::GetWindowPos();

    if(IMVEC2_DIFF(sz, m_oldSize) || IMVEC2_DIFF(pos, m_oldPos)) {
        RD_Rect vpnew{};
        vpnew.x = pos.x;
        vpnew.y = pos.y;
        vpnew.w = sz.x;
        vpnew.h = sz.y;

        winsys->SetViewport(vpnew);
    }

    m_oldSize = sz;
    m_oldPos = pos;

    ImGui::End();

    ImGui::PopStyleVar();
    ImGui::PopStyleColor();

    ImGui::Begin("Node Graph");
    m_node_editor->RenderGraph();
    ImGui::End();

    ImGui::Begin("Properties");

    m_node_editor->RenderNodeProperties();

    ImGui::End();
}

void OmbrageUI::UI::DockSpaceHandling() {
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    const ImGuiViewport* imvp = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(imvp->WorkPos);
    ImGui::SetNextWindowSize(imvp->WorkSize);
    ImGui::SetNextWindowViewport(imvp->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, ImVec4{0.0, 0.0, 0.0, 0.0});
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4{0.0, 0.0, 0.0, 0.0});

    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::Begin("OmbrageUI Editor", nullptr, window_flags);

    ImGuiID dock_id = ImGui::GetID("OmbrageDockSpace");
    ImGui::DockSpace(dock_id, ImVec2(0.0f, 0.0f));

    ImGui::End();
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(2);

}
