#include "RD_RenderingPipeline.h"

RD_RenderingPipeline_PBR::RD_RenderingPipeline_PBR(std::shared_ptr<RD_API>& api) {
    m_api = api;
}

RD_RenderingPipeline_PBR::~RD_RenderingPipeline_PBR() {

}

bool RD_RenderingPipeline_PBR::InitRenderingPipeline(std::string enginePath) {
    int w, h;
    w = m_api->GetWindowingSystem()->GetWidth();
    h = m_api->GetWindowingSystem()->GetHeight();

    m_renderSurface = std::make_shared<RD_Quad>(m_api);

    //FRAMEBUFFER
    RD_Attachment color{};
    color.format = IMGFORMAT_RGBA;
    color.do_clear = true;
    color.is_swapchain_attachment = false;
    color.sample_count = 1;

    RD_Attachment colorf{};
    colorf.format = IMGFORMAT_RGBA16F;
    colorf.do_clear = true;
    colorf.is_swapchain_attachment = false;
    colorf.sample_count = 1;

    RD_Attachment depth{};
    depth.format = IMGFORMAT_DEPTH;
    depth.do_clear = true;
    depth.is_swapchain_attachment = false;
    depth.sample_count = 1;

    /*
     *
     * FRAMEBUFFER COMPOSITION :
     *  - Color
     *  - Normal
     *  - Frag Position
     *  - Depth
     *
     */

    m_rpassGBuff = m_api->CreateRenderPass({color, colorf, colorf, depth}, static_cast<float>(w), static_cast<float>(h));
    m_rpassGBuff->BuildRenderpass(m_api.get(), false);

    m_rpassLight = m_api->CreateRenderPass({color}, static_cast<float>(w), static_cast<float>(h));
    m_rpassLight->BuildRenderpass(m_api.get(), false);

    auto tex = m_rpassLight->GetAttachment(0);
    m_api->GetWindowingSystem()->SetPresentTexture(tex);

    //UNIFORMS
    m_camModel = m_api->CreateUniformBuffer(0);
    m_camModel->BuildAndAllocateBuffer(16 * sizeof(float) * 3);

    m_casterCount = m_api->CreateUniformBuffer(5);
    m_casterCount->BuildAndAllocateBuffer(sizeof(RD_CasterCount));

    m_dlights = m_api->CreateUniformBuffer(3);
    m_dlights->BuildAndAllocateBuffer(10 * sizeof(GLSLDirLight));

    m_plights = m_api->CreateUniformBuffer(4);
    m_plights->BuildAndAllocateBuffer(300 * sizeof(GLSLPointLight));

    std::shared_ptr<RD_ShaderLoader> base_shader = m_api->CreateShader();
    base_shader->CompileShaderFromFile(enginePath + "/shaders/bin/base.vspv", enginePath + "/shaders/bin/base.fspv");

    std::shared_ptr<RD_ShaderLoader> light_shader = m_api->CreateShader();
    light_shader->CompileShaderFromFile(enginePath + "/shaders/bin/sc_blit.vspv", enginePath + "/shaders/bin/lighting_pass.fspv");

    m_plineGBuff = m_api->CreatePipeline(m_rpassGBuff, base_shader);
    m_plineGBuff->RegisterUniformBuffer(m_camModel);
    m_plineGBuff->BuildPipeline();

    m_plineLight = m_api->CreatePipeline(m_rpassLight, light_shader);

    auto colorpass = m_rpassGBuff->GetAttachment(0);
    auto normpass = m_rpassGBuff->GetAttachment(1);
    auto pospass = m_rpassGBuff->GetAttachment(2);

    m_plineLight->RegisterTexture(colorpass, 0);
    m_plineLight->RegisterTexture(normpass, 1);
    m_plineLight->RegisterTexture(pospass, 2);

    m_plineLight->RegisterUniformBuffer(m_dlights);
    m_plineLight->RegisterUniformBuffer(m_plights);
    m_plineLight->RegisterUniformBuffer(m_casterCount);

    m_plineLight->BuildPipeline();

    return true;
}

void RD_RenderingPipeline_PBR::RenderScene(std::vector<std::shared_ptr<RD_Mesh>>& sceneData, std::shared_ptr<RD_Camera> cam) {
    if(sceneData.empty()) {
        return;
    }

    cam->PushToUniform(m_camModel);
    m_plineGBuff->Bind();

    for(auto& v : sceneData) {
        m_camModel->PartialFillBufferData(v->GetTransform().GetPTR(), 16 * sizeof(float), 2 * 16 * sizeof(float));
        m_plineGBuff->DrawIndexedVertexBuffer(v->GetVertexBuffer());
    }

    m_plineGBuff->Unbind();

    m_plineLight->Bind();
    m_plineLight->DrawIndexedVertexBuffer(m_renderSurface->GetVertexBuffer());
    m_plineLight->Unbind();
}

void RD_RenderingPipeline_PBR::Resize(int w, int h) {
    m_rpassGBuff->SetRenderpassSize(m_api.get(), w, h);
    m_rpassLight->SetRenderpassSize(m_api.get(), w, h);
    m_api->GetWindowingSystem()->SetPresentTexture(m_rpassLight->GetAttachment(0));

    m_plineGBuff->RebuildPipeline();

    auto colorpass = m_rpassGBuff->GetAttachment(0);
    auto normpass = m_rpassGBuff->GetAttachment(1);
    auto pospass = m_rpassGBuff->GetAttachment(2);

    m_plineLight->PurgeTextures();
    m_plineLight->RegisterTexture(colorpass, 0);
    m_plineLight->RegisterTexture(normpass, 1);
    m_plineLight->RegisterTexture(pospass, 2);
    m_plineLight->RebuildPipeline();
}

void RD_RenderingPipeline_PBR::PushDirLight(std::shared_ptr<RD_DirLight> dlight, int index) {
    dlight->PushToUniform(m_dlights, index);
}

void RD_RenderingPipeline_PBR::PushCasterCount(RD_CasterCount &ccount) {
    m_casterCount->FillBufferData(&ccount);
}

void RD_RenderingPipeline_PBR::PushPointLight(std::shared_ptr<RD_PointLight> plight, int index) {
    plight->PushToUniform(m_plights, index);
}

std::shared_ptr<RD_RenderPass> RD_RenderingPipeline_PBR::GetBaseRenderpass() {
    return m_rpassGBuff;
}