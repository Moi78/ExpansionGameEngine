#include "UI.h"
#include "MathNodes.h"

OmbrageUI::UI::UI(EXP_Game *game) {
    m_game = game;

    m_oldSize = {0, 0};
    m_oldPos = {0, 0};

    m_node_editor = std::make_unique<NodeGraph_UI>();

    m_compiler = std::make_shared<SpirvCompiler>();

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

    if(ImGui::IsKeyPressed(ImGuiKey_R, false)) {
        if(!ImGui::IsPopupOpen("Recompiling")) {
            ImGui::OpenPopup("Recompiling");

            ImVec2 winSize = ImGui::GetMainViewport()->Size;
            ImVec2 popupSize{};

            auto imwin = ImGui::FindWindowByName("Recompiling");
            if(imwin) {
                popupSize = imwin->Size;
            }

            ImVec2 finalPos = (winSize / 2) - (popupSize / 2);

            ImGui::SetNextWindowPos(finalPos);
        }

        m_cstate = DEPS_LOAD;
        if(CompileLoadDeps()) {
            m_cstate = COMPILING_SHADER;
            if(CompileShader()) {
                m_cstate = DONE;
            }
        }
    }
    if(ImGui::BeginPopupModal(
            "Recompiling", nullptr,
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar)
    ) {
        ImGui::Text("Recompiling...");

        if(m_cstate == DEPS_LOAD) {
            ImGui::Text("Loading deps...");
        } else if(m_cstate == COMPILING_SHADER) {
            ImGui::Text("Compiling shader...");
        } else if(m_cstate == DONE) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

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

bool OmbrageUI::UI::CompileLoadDeps() {
    m_compiler.reset();
    m_compiler = std::make_unique<SpirvCompiler>();

    for(auto& n : m_node_editor->GetNodes()) {
        std::string dep = n->GetNodeFunctionName();
        if(!((dep == "err") || (dep == "shadernode") || (dep == "ctant"))) {
            m_compiler->LoadDependency(dep, "ombrage_content/spv_lib/");
        }
    }

    return true;
}

bool OmbrageUI::UI::CompileShader() {
    m_compiler->MakeEntry();

    // Make function graph for each outputs

    std::vector<HLTypes> layout = {
            HLTypes::FLOATPTRO,
            HLTypes::VECTOR4PTRO,
            HLTypes::VECTOR4PTRO,
            HLTypes::VECTOR4PTRO,
            HLTypes::VECTOR4PTRO,
    };
    m_compiler->SetShaderLayout(layout);

    std::vector<std::shared_ptr<Node>>& nodes = m_node_editor->GetNodes();

    // Const analysis
    for(auto& n : nodes) {
        if(n->isConst()) {
            auto ct = std::dynamic_pointer_cast<Constant>(n);
            ct->MakeCtant();

            m_compiler->RegisterConstant(ct->ctant);
        }
    }

    std::shared_ptr<Node> rootNode = nodes[0];
    auto evaluated = rootNode->EvalFrom(3, m_compiler);
    auto var = m_compiler->GetLayoutVariable(0);

    if(evaluated->t == FuncGraphElemType::FUNCTION) {
        auto funcall = std::reinterpret_pointer_cast<HighLevelFunCall>(evaluated)->funCall;
        funcall->target = var;
        funcall->noStore = false;
    } else {
        auto ctant = std::reinterpret_pointer_cast<HighLevelCtant>(evaluated)->ctant;
        auto opStore = std::make_shared<SpOpStoreCtant>(var, ctant);
        m_compiler->AddOpToEntry(opStore);
    }

    std::vector<uint32_t> progBin = m_compiler->CompileAll();

    std::ofstream f("test.bin", std::ios::binary);
    f.write((char*)progBin.data(), sizeof(uint32_t) * progBin.size());
    f.close();

    return true;
}
