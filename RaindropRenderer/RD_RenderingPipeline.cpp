#include "RD_RenderingPipeline.h"

RD_RenderingPipeline_PBR::RD_RenderingPipeline_PBR(std::shared_ptr<RD_API>& api) {
    m_api = api;
}

RD_RenderingPipeline_PBR::~RD_RenderingPipeline_PBR() {

}

bool RD_RenderingPipeline_PBR::InitRenderingPipeline() {
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

    m_dlights = m_api->CreateUniformBuffer(3);
    m_dlights->BuildAndAllocateBuffer(10 * sizeof(GLSLDirLight));

    m_cam = std::make_unique<RD_Camera>(m_api, 30.0f, vec3(-5.0f, -2.5f, 2.0f), vec3(0.0f, 0.0f, 0.0f), 0.1f, 1000.0f);

    std::shared_ptr<RD_ShaderLoader> base_shader = m_api->CreateShader();
    base_shader->CompileShaderFromFile("shaders/bin/base.vspv", "shaders/bin/base.fspv");

    std::shared_ptr<RD_ShaderLoader> light_shader = m_api->CreateShader();
    light_shader->CompileShaderFromFile("shaders/bin/sc_blit.vspv", "shaders/bin/lighting_pass.fspv");

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

    m_plineLight->BuildPipeline();

    RD_DirLight test = RD_DirLight(vec3(1.0f, 0.0f, 0.0f), 0.5f, vec3(1.0f, 0.5f, 1.0f));
    test.PushToUniform(m_dlights, 0);

    RD_DirLight test2 = RD_DirLight(vec3(-1.0f, 0.0f, -0.5f), 1.0f, vec3(0.5f, 1.0f, 0.5f));
    test2.PushToUniform(m_dlights, 1);

    return true;
}

void RD_RenderingPipeline_PBR::RenderScene(std::vector<std::shared_ptr<RD_Mesh>>& sceneData) {
    if(sceneData.empty()) {
        return;
    }

    m_cam->PushToUniform(m_camModel);
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
    m_api->GetWindowingSystem()->SetPresentTexture(m_rpassGBuff->GetAttachment(0));

    m_plineGBuff->RebuildPipeline();
}