#include "EXP_Game.h"

EXP_Game::EXP_Game(std::shared_ptr<RaindropRenderer> rndr, EXP_GameInfo gameinfo) {
    m_rndr = rndr;
    m_gameinfo = std::move(gameinfo);
}

EXP_Game::~EXP_Game() {

}

bool EXP_Game::InitEngine() {
    if(!m_rndr->InitRenderer(m_gameinfo.RootEngineContentDir)) {
        return false;
    }

    std::shared_ptr<RD_ShaderLoader> shader = m_rndr->GetAPI()->CreateShader();
    shader->CompileShaderFromFile(m_gameinfo.RootEngineContentDir + "/shaders/bin/base.vspv", m_gameinfo.RootEngineContentDir + "/shaders/bin/base.fspv");

    std::shared_ptr<RD_ShaderLoader> shader2 = m_rndr->GetAPI()->CreateShader();
    shader2->CompileShaderFromFile(m_gameinfo.RootEngineContentDir + "/shaders/bin/base.vspv", m_gameinfo.RootEngineContentDir + "/shaders/bin/base_two.fspv");

    std::shared_ptr<RD_Pipeline> pline = m_rndr->CreatePipeline(shader);
    std::shared_ptr<RD_Pipeline> pline2 = m_rndr->CreatePipeline(shader2);

    std::shared_ptr<RD_Material> mat2 = std::make_shared<RD_Material>(pline2);
    m_rndr->RegisterMaterial(mat2);

    std::shared_ptr<RD_Material> mat = std::make_shared<RD_Material>(pline);
    m_rndr->RegisterMaterial(mat);

    std::shared_ptr<RD_Mesh> mesh2 = std::make_shared<RD_Mesh>(m_rndr->GetAPI(), vec3(0.0f, 0.0f, 1.5f), vec3(), vec3(1.0f, 1.0f, 1.0f));
    mesh2->LoadMesh(m_gameinfo.RootGameContentDir + "/mdl/monkey.msh");

    std::shared_ptr<RD_Mesh> mesh = std::make_shared<RD_Mesh>(m_rndr->GetAPI(), vec3(), vec3(), vec3(1.0f, 1.0f, 1.0f));
    mesh->LoadMesh(m_gameinfo.RootGameContentDir + "/mdl/monkey.msh");

    mat2->RegisterMesh(mesh2);
    mat->RegisterMesh(mesh);

    std::shared_ptr<RD_DirLight> dlight = std::make_shared<RD_DirLight>(vec3(0.0f, 0.0f, -1.0f), 1.0f, vec3(1.0f));
    m_rndr->RegisterDirLight(dlight);

    std::shared_ptr<RD_PointLight> plight = std::make_shared<RD_PointLight>(vec3(0.0f, -2.0f, -1.5f), 1.0f, vec3(0.0f, 1.0f, 0.5f), 10.0f);
    m_rndr->RegisterPointLight(plight);

    std::shared_ptr<RD_Camera> cam = std::make_shared<RD_Camera>(m_rndr->GetAPI(), 30.0f, vec3(-2.0f, -2.0f, 1.0f), vec3(), 0.1f, 100.0f);
    m_rndr->SetCurrentCamera(cam);

    return true;
}

void EXP_Game::RunGame() {
    while(!m_rndr->WantToClose()) {
        m_rndr->UpdateWindow();
        m_rndr->RenderScene();
    }
}