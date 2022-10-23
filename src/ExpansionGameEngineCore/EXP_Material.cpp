#include "EXP_Material.h"

EXP_Material::EXP_Material(EXP_Game* game) {
    m_game = game;
}

EXP_Material::~EXP_Material() {

}

bool EXP_Material::LoadMaterial(std::string material_file) {
    std::ifstream file(m_game->GetGameContentPath() + material_file);
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

    auto shaderLoader = m_game->GetRenderer()->GetAPI()->CreateShader();
    bool result = shaderLoader->CompileShaderFromFile(
            m_game->GetEngineContentPath() + "/shaders/bin/base.vspv",
            m_game->GetGameContentPath() + root["spirv_file"].asString()
    );

    if(!result) {
        std::cout << "Failed to load shader file " << root["spirv_file"].asString() << std::endl;
        return false;
    }

    auto pline = m_game->GetRenderer()->CreatePipeline(shaderLoader);
    m_mat = std::make_shared<RD_Material>(pline);

    m_game->GetRenderer()->RegisterMaterial(m_mat);

    return true;
}

void EXP_Material::RegisterMesh(std::shared_ptr<RD_Mesh> smesh) {
    m_mat->RegisterMesh(smesh);
}