#include "EXP_Material.h"

EXP_Material::EXP_Material(EXP_Game* game) {
    m_game = game;
}

EXP_Material::~EXP_Material() {

}

bool EXP_Material::LoadMaterial(std::string material_file, bool enginePath) {
    std::string rootPath = enginePath ? m_game->GetEngineContentPath() : m_game->GetGameContentPath();
    std::ifstream file(rootPath + material_file);
    if(!file.is_open()) {
        return false;
    }

    Json::Value root;
    JSONCPP_STRING errs;

    Json::CharReaderBuilder builder;
    builder["collecoComment"] = false;

    if(!Json::parseFromStream(builder, file, &root, &errs)) {
        std::cerr << "ERROR: Cannot parse " << material_file << ". " << errs << std::endl;
        return false;
    }

    bool hasCustomVert = root.get("custom_vert", false).asBool();
    bool isEngineMat = root.get("engine_mat", false).asBool();

    std::string vertPath;
    if(hasCustomVert) {
        vertPath += isEngineMat ? m_game->GetEngineContentPath() : m_game->GetGameContentPath();

        std::string customVertPath = root.get("spirv_file_vert", "").asString();
        if(customVertPath == "") {
            vertPath =  m_game->GetEngineContentPath() + "/shaders/bin/base.vspv";
        } else {
            vertPath += customVertPath;
        }
    } else {
        vertPath = m_game->GetEngineContentPath() + "/shaders/bin/base.vspv";
    }

    auto shaderLoader = m_game->GetRenderer()->GetAPI()->CreateShader();
    bool result = shaderLoader->CompileShaderFromFile(
            vertPath,
            rootPath + root["spirv_file"].asString()
    );

    if(!result) {
        std::cout << "Failed to load shader file " << root["spirv_file"].asString() << std::endl;
        return false;
    }

    bool isUI = root.get("ui_material", false).asBool();

    std::shared_ptr<RD_Pipeline> pline;
    if(isUI) {
        auto rpass = m_game->GetGuiManager()->GetRenderPass();
        pline = m_game->GetRenderer()->GetAPI()->CreatePipeline(rpass, shaderLoader);
        pline->ConfigurePushConstant(8 * sizeof(float));
        pline->RegisterUniformBuffer(m_game->GetRenderer()->GetAPI()->GetWindowingSystem()->GetScreenSizeBuffer());
    } else {
        pline = m_game->GetRenderer()->CreatePipeline(shaderLoader);
    }

    for(auto& t : root["texs"]) {
        std::shared_ptr<RD_Texture> tex = m_game->GetRenderer()->GetAPI()->CreateTexture();
        if(!tex->LoadTextureFromFile(rootPath + t["path"].asString())) {
            continue;
        }

        pline->RegisterTexture(tex, t["binding"].asInt());
    }
    pline->BuildPipeline();

    m_mat = std::make_shared<RD_Material>(pline);
    m_game->GetRenderer()->RegisterMaterial(m_mat);

    return true;
}

void EXP_Material::RegisterMesh(std::shared_ptr<RD_Mesh> smesh) {
    m_mat->RegisterMesh(smesh);
}

std::shared_ptr<RD_Pipeline> EXP_Material::GetPipeline() {
    return m_mat->GetRawPipeline();
}
