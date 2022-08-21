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

    RD_Attachment att{};
    att.format = IMGFORMAT_RGBA;
    att.do_clear = true;
    att.is_swapchain_attachment = false;
    att.sample_count = 1;

    RD_Attachment depth{};
    depth.format = IMGFORMAT_DEPTH;
    depth.do_clear = true;
    depth.is_swapchain_attachment = false;
    depth.sample_count = 1;

    m_rpass = m_api->CreateRenderPass({att, att, depth}, static_cast<float>(w), static_cast<float>(h));
    m_rpass->BuildRenderpass(m_api.get(), false);

    auto tex = m_rpass->GetAttachment(0);
    m_api->GetWindowingSystem()->SetPresentTexture(tex);

    m_cam = std::make_unique<RD_Camera>(m_api, 30.0f, vec3(-5.0f, -2.5f, 2.0f), vec3(0.0f, 0.0f, 0.0f), 0.1f, 1000.0f);

    std::shared_ptr<RD_ShaderLoader> shader = m_api->CreateShader();
    shader->CompileShaderFromFile("shaders/bin/base.vspv", "shaders/bin/base.fspv");

    m_plineGBuff = m_api->CreatePipeline(m_rpass, shader);

    m_cam->SetupPipeline(m_plineGBuff);

    m_plineGBuff->BuildPipeline();

    return true;
}

void RD_RenderingPipeline_PBR::RenderScene(std::vector<std::shared_ptr<RD_IndexedVertexBuffer>>& sceneData) {
    m_plineGBuff->Bind();

    for(auto& v : sceneData) {
        m_plineGBuff->DrawIndexedVertexBuffer(v);
    }

    m_plineGBuff->Unbind();
}