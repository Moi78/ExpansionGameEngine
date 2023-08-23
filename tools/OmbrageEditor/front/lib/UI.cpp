#include "UI.h"
#include "MathNodes.h"

OmbrageUI::UI::UI(EXP_Game *game) {
    m_game = game;

    m_oldSize = {0, 0};
    m_oldPos = {0, 0};

    m_node_editor = std::make_unique<NodeGraph_UI>();

    m_compiler = std::make_shared<SpirvCompiler>();

    m_cstate = CompileState::DONE;

    m_isFileLoaded = false;
    m_isFileSaved = false;

    m_projContentRoot = "/";
    std::vector<std::string> args = game->GetGameArguments();
    for(int i = 0; i < args.size(); i++) {
        if(args[i] == "--project-content") {
            if((i + 1) < args.size()) {
                m_projContentRoot = args[i + 1];
            }
        }
    }

    if(!std::filesystem::exists(m_projContentRoot) || !std::filesystem::is_directory(m_projContentRoot)) {
        std::cerr << "ERROR: Bad project content root. Please give an existing directory." << std::endl;
        m_projContentRoot = "/";
    }

    std::cout << "Using " << m_projContentRoot << " as project content root" << std::endl;

    auto rootNode = std::make_shared<ShaderNode>(0);
    m_node_editor->AddNode(rootNode);

    m_node_editor->AddNodeToCatalog("Basic Maths", "Add", [](int id) { return std::make_shared<OmbrageNodes::AddNode>(id); });
    m_node_editor->AddNodeToCatalog("Basic Maths", "Multiply", [](int id) { return std::make_shared<OmbrageNodes::MultNode>(id); });

    m_node_editor->AddNodeToCatalog("Constants", "Const Float", [](int id) { return std::make_shared<OmbrageNodes::ConstFloatNode>(id); });
    m_node_editor->AddNodeToCatalog("Constants", "Const Vec2", [](int id) { return std::make_shared<OmbrageNodes::ConstVec2Node>(id); });
    m_node_editor->AddNodeToCatalog("Constants", "Const Vec3", [](int id) { return std::make_shared<OmbrageNodes::ConstVec3Node>(id); });
    m_node_editor->AddNodeToCatalog("Constants", "Const Vec4", [](int id) { return std::make_shared<OmbrageNodes::ConstVec4Node>(id); });

    m_node_editor->AddNodeToCatalog("Texture", "Sample Texture", [](int id) { return std::make_shared<OmbrageNodes::TextureNode>(id); });

    m_node_editor->AddNodeToCatalog("Variables", "Normal", [](int id) { return std::make_shared<OmbrageNodes::NormalNode>(id); });
    m_node_editor->AddNodeToCatalog("Variables", "Position", [](int id) { return std::make_shared<OmbrageNodes::PosNode>(id); });
    m_node_editor->AddNodeToCatalog("Variables", "UV Coordinates", [](int id) { return std::make_shared<OmbrageNodes::UVNode>(id); });

    m_node_editor->AddNodeToCatalog("Vector", "Combine", [](int id) { return std::make_shared<OmbrageNodes::CombineNode>(id); });
    m_node_editor->AddNodeToCatalog("Vector", "Split", [](int id) { return std::make_shared<OmbrageNodes::SplitNode>(id); });
    m_node_editor->AddNodeToCatalog("Vector", "XYZ", [](int id) { return std::make_shared<OmbrageNodes::XYZNode>(id); });
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
    MainMenuBar();

    ImGuiIO& io = ImGui::GetIO();
    if(io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_S, false)) {
        Save();
    }

    if(io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_O, false)) {
        Open();
    }

    if(io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_E, false)) {
        Export();
    }

    if(io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_N, false)) {
        New();
    }

    if(ImGui::IsKeyPressed(ImGuiKey_R, false) && m_node_editor->IsEditorHovered()) {
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
        } else {
            m_cstate = DONE;
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
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

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

void OmbrageUI::UI::MainMenuBar() {
    ImGui::BeginMainMenuBar();

    if(ImGui::BeginMenu("File")) {
        if(ImGui::MenuItem("New", "CTRL+N")) {
            New();
        }

        if(ImGui::MenuItem("Open", "CTRL+O")) {
            Open();
        }

        ImGui::Separator();

        if(ImGui::MenuItem("Save", "CTRL+S")) {
            Save();
        }

        if(ImGui::MenuItem("Save As", "CTRL+MAJ+S")) {
            SaveAs();
        }

        ImGui::Separator();

        if(ImGui::MenuItem("Export", "CTRL+E")) {
            Export();
        }

        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
}

bool OmbrageUI::UI::CompileLoadDeps() {
    m_compiler.reset();
    m_compiler = std::make_unique<SpirvCompiler>();

    for(auto& n : m_node_editor->GetNodes()) {
        std::string dep = n->GetNodeFunctionName();
        if(!((dep == "err") || (dep == "shadernode") || (dep == "ctant"))) {
            if(!m_compiler->LoadDependency(dep, "ombrage_content/spv_lib/")) {
                return false;
            }
        }
    }

    return true;
}

bool OmbrageUI::UI::CompileShader() {
    m_compiler->MakeEntry();

    // Make function graph for each outputs
    std::vector<std::shared_ptr<Node>>& nodes = m_node_editor->GetNodes();
    auto rootNode = std::reinterpret_pointer_cast<ShaderNode>(nodes[0]);

    m_compiler->SetShaderLayout(rootNode->GetShaderLayout());
    m_compiler->SetShaderInputs({HLTypes::VECTOR2PTRI, HLTypes::VECTOR3PTRI, HLTypes::VECTOR3PTRI});

    // Const & Tex analysis
    for(auto& n : nodes) {
        if(n->isConst()) {
            auto ct = std::dynamic_pointer_cast<Constant>(n);
            ct->MakeCtant();

            m_compiler->RegisterConstant(ct->ctant);
        } else if(n->doHoldTexture()) {
            m_compiler->RequireType(HLTypes::SAMPLED_IMAGE_PTRUC);

            std::cout << "Loading texture " << n->GetTexturePath() << std::endl;
            m_compiler->RegisterTexture(n->GetTexturePath());
        }
    }

    for(int i = 0; i < rootNode->GetNodeSize(); i++) {
        auto evaluated = rootNode->EvalFrom(i, m_compiler);

        std::shared_ptr<SpirvDataWrapperBase> toStore;
        if(evaluated->t == FuncGraphElemType::NONE) {
            toStore = rootNode->GetDefault(m_compiler, i);

        } else if (evaluated->t == FuncGraphElemType::FUNCTION) {
            auto funcall = std::reinterpret_pointer_cast<HighLevelFunCall>(evaluated)->funCall;
            auto funcall_wrapper = std::make_shared<SpirvDataWrapperFunRet>();
            funcall_wrapper->fcall = funcall;
            funcall_wrapper->stcl = StorageClass::Function;

            toStore = funcall_wrapper;
        } else {
            toStore = std::reinterpret_pointer_cast<HighLevelCtant>(evaluated)->ctant;
        }

        std::vector<std::shared_ptr<SpirvOperation>> ops = rootNode->GetStoreOpForNode(m_compiler, i, toStore);
        for (auto &op: ops) {
            m_compiler->AddOpToEntry(op);
        }
    }

    std::vector<uint32_t> progBin = m_compiler->CompileAll();
    m_lastCompiledData = progBin;

    std::shared_ptr<RD_ShaderLoader> shaderLoader = m_game->GetRenderer()->GetAPI()->CreateShader();
    shaderLoader->LoadFragBinary(m_game->GetEngineContentPath() + "/shaders/bin/base.vspv", progBin);

    std::shared_ptr<EXP_Level> lvl = m_game->GetCurrentLevel();
    auto model = lvl->GetCastedActorByName<EXP_StaticMeshActor>("sphere");

    std::shared_ptr<EXP_Material> mat = model->GetMeshComponent()->GetMeshMaterial();
    mat->GetPipeline()->PurgeTextures();
    mat->GetPipeline()->SwapShader(shaderLoader);

    m_texmap.clear();
    m_texmap = m_compiler->GetTextureNames();

    auto api = m_game->GetRenderer()->GetAPI();
    for(auto& t : m_texmap) {
        auto tex = api->CreateTexture();
        tex->LoadTextureFromFile(m_projContentRoot + t.first);

        mat->GetPipeline()->RegisterTexture(tex, t.second);
    }

    mat->GetPipeline()->RebuildPipeline();

    return true;
}

void OmbrageUI::UI::Save() {
    std::string filePath;
    if(!m_isFileLoaded) {
        filePath = pfd::save_file(
                "Save File", m_projContentRoot, {"Expansion Material Files", "*.exmtl"}
        ).result();

        m_savepath = filePath;
        m_isFileLoaded = true;

    } else {
        filePath = m_savepath;
    }

    if(!filePath.empty()) {
        m_node_editor->SaveGraphToFile(filePath);
    }
}

void OmbrageUI::UI::Open() {
    std::vector<std::string> filePath = pfd::open_file("Open File", m_projContentRoot, {"Expansion Material Files", "*.exmtl"}).result();
    if(!filePath.empty()) {
        m_node_editor->LoadGraphFromFile(filePath[0]);

        m_isFileLoaded = true;
        m_savepath = filePath[0];
    }
}

void OmbrageUI::UI::Export() {
    if(CompileLoadDeps()) {
        if(!CompileShader()) {
            std::cerr << "ERROR: Failed to compile shader." << std::endl;
            return;
        }
    } else {
        return;
    }

    std::string filePath = pfd::save_file("Export Shader", m_projContentRoot, {"JSON Files", "*.json"}).result();
    if(filePath.empty()) {
        return;
    }

    Json::Value root;
    root["embedded_bin"] = true;

    for(int i = 0; i < m_lastCompiledData.size(); i++) {
        root["spirv_bin"][i] = m_lastCompiledData[i];
    }

    int i = 0;
    for(auto& t : m_texmap) {
        Json::Value tex;
        tex["path"] = t.first;
        tex["binding"] = t.second;

        root["texs"][i] = tex;

        i++;
    }

    std::ofstream file(filePath, std::ios::binary);
    if(!file.is_open()) {
        std::cerr << "ERROR: Unable to create file." << std::endl;
    }

    file << root;
    file.close();
}

void OmbrageUI::UI::New() {
    auto ret = pfd::message("New Material", "Do you really want to create a new blank material ?", pfd::choice::yes_no, pfd::icon::question).result();
    if(ret == pfd::button::yes) {
        m_node_editor->ResetEditor();

        auto shaderNode = std::make_shared<ShaderNode>(0);
        m_node_editor->AddNode(shaderNode);

        ImNodes::SetNodeEditorSpacePos(0, ImVec2(0, 0));
    }
}

void OmbrageUI::UI::SaveAs() {

}
