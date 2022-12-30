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
    m_sync = std::reinterpret_pointer_cast<RD_RenderSynchronizer_Vk>(m_api->CreateRenderSynchronizer());

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

    RD_Attachment monocolorf{};
    monocolorf.format = IMGFORMAT_R32F;
    monocolorf.do_clear = true;
    monocolorf.is_swapchain_attachment = false;
    monocolorf.sample_count = 1;

    /*
     *
     * FRAMEBUFFER COMPOSITION :
     *  - Color (0)
     *  - Normal (1)
     *  - Frag Position (2)
     *  - MetRoughAO (3)
     *  - Sheen (4)
     *  - Depth (5)
     *
     */

    m_rpassGBuff = m_api->CreateRenderPass({color, colorf, colorf, colorf, colorf, depth}, static_cast<float>(w), static_cast<float>(h));
    m_rpassGBuff->BuildRenderpass(m_api.get(), false);

    m_rpassLight = m_api->CreateRenderPass({colorf}, static_cast<float>(w), static_cast<float>(h));
    m_rpassLight->BuildRenderpass(m_api.get(), false);

    m_rpassShadowDepth = m_api->CreateRenderPass({depth, monocolorf}, SHADOW_RES, SHADOW_RES);
    m_rpassShadowDepth->BuildRenderpass(m_api.get(), false);

    auto tex = m_rpassLight->GetAttachment(0);
    m_api->GetWindowingSystem()->SetPresentTexture(tex);

    m_rpassSblur = m_api->CreateRenderPass({depth, monocolorf}, SHADOW_RES, SHADOW_RES);
    m_rpassSblur->BuildRenderpass(m_api.get(), false);

    m_sblur = m_api->CreateOrphanFramebuffer(m_rpassSblur, {depth, monocolorf}, SHADOW_RES, SHADOW_RES);
    m_sblur->BuildFramebuffer(m_api.get());

    //UNIFORMS
    m_camModel = m_api->CreateUniformBuffer(0);
    m_camModel->BuildAndAllocateBuffer(16 * sizeof(float) * 3);

    m_models = m_api->CreateUniformBuffer(1);
    m_models->BuildAndAllocateBuffer(16 * sizeof(float) * 256);

    m_casterCount = m_api->CreateUniformBuffer(5);
    m_casterCount->BuildAndAllocateBuffer(sizeof(RD_CasterCount));

    m_dlights = m_api->CreateUniformBuffer(3);
    m_dlights->BuildAndAllocateBuffer(10 * sizeof(GLSLDirLight));

    m_plights = m_api->CreateUniformBuffer(4);
    m_plights->BuildAndAllocateBuffer(300 * sizeof(GLSLPointLight));

    m_camData = m_api->CreateUniformBuffer(6);
    m_camData->BuildAndAllocateBuffer(10 * sizeof(float));

    vec2 scrSize = vec2(m_api->GetWindowingSystem()->GetWidth(), m_api->GetWindowingSystem()->GetHeight());
    m_camData->PartialFillBufferData(scrSize.GetData(), 2 * sizeof(float), 4 * 2 * sizeof(float));

    m_lightMat = m_api->CreateUniformBuffer(7);
    m_lightMat->BuildAndAllocateBuffer(16 * 10 * sizeof(float));

    m_bonesBuffer = m_api->CreateUniformBuffer(1);
    m_bonesBuffer->BuildAndAllocateBuffer(1024 * 16 * sizeof(float));

    std::shared_ptr<RD_ShaderLoader> base_shader = m_api->CreateShader();
    base_shader->CompileShaderFromFile(enginePath + "/shaders/bin/base.vspv", enginePath + "/shaders/bin/base.fspv");

    std::shared_ptr<RD_ShaderLoader> light_shader = m_api->CreateShader();
    light_shader->CompileShaderFromFile(enginePath + "/shaders/bin/sc_blit.vspv", enginePath + "/shaders/bin/lighting_pass.fspv");

    std::shared_ptr<RD_ShaderLoader> shadowDepth_shader = m_api->CreateShader();
    shadowDepth_shader->CompileShaderFromFile(enginePath + "/shaders/bin/shadow_depth.vspv", enginePath + "/shaders/bin/shadow_depth.fspv");

    std::shared_ptr<RD_ShaderLoader> monoblur_shader = m_api->CreateShader();
    monoblur_shader->CompileShaderFromFile(enginePath + "/shaders/bin/sc_blit.vspv", enginePath + "/shaders/bin/mono_blur.fspv");

    m_plineSblur = m_api->CreatePipeline(m_rpassSblur, monoblur_shader);
    m_plineSblur->RegisterTexture(m_rpassShadowDepth->GetAttachment(1), 0);
    m_plineSblur->ConfigurePushConstant(2 * sizeof(float));
    m_plineSblur->BuildPipeline();

    m_plineSblur_b = m_api->CreatePipeline(m_rpassSblur, monoblur_shader);
    m_plineSblur_b->RegisterTexture(m_sblur->GetAttachment(1), 0);
    m_plineSblur_b->ConfigurePushConstant(2 * sizeof(float));
    m_plineSblur_b->BuildPipeline();

    m_plineShadowDepth = m_api->CreatePipeline(m_rpassShadowDepth, shadowDepth_shader);
    m_plineShadowDepth->ConfigurePushConstant(16 * sizeof(float) + sizeof(uint32_t));
    m_plineShadowDepth->RegisterUniformBuffer(m_lightMat);
    m_plineShadowDepth->SetCullMode(RD_CullMode::CM_NONE);
    m_plineShadowDepth->BuildPipeline();

    m_plineLight = m_api->CreatePipeline(m_rpassLight, light_shader);

    std::vector<std::shared_ptr<RD_Texture>> tArray = {};
    for(auto& s : m_depthFBs) {
        tArray.push_back(s->GetAttachment(1));
    }

    m_plineLight->RegisterTextureArray(tArray, 20);

    m_plineLight->RegisterUniformBuffer(m_lightMat);

    for(int i = 0; i < 5; i++) {
        m_plineLight->RegisterTexture(m_rpassGBuff->GetAttachment(i), i + 10);
    }

    m_plineLight->RegisterUniformBuffer(m_dlights);
    m_plineLight->RegisterUniformBuffer(m_plights);
    m_plineLight->RegisterUniformBuffer(m_casterCount);
    m_plineLight->RegisterUniformBuffer(m_camData);

    m_plineLight->BuildPipeline();

    return true;
}

void RD_RenderingPipeline_PBR::RenderScene(std::vector<std::shared_ptr<RD_Material>>& sceneData, std::shared_ptr<RD_Camera> cam) {
    if(sceneData.empty()) {
        return;
    }

    cam->PushToUniform(m_camModel);
    cam->PushCamDataToUniform(m_camData);

    m_sync->Start();
    m_rpassGBuff->BeginRenderpass(m_sync);

    for(auto& m : sceneData) {
        m->RenderMeshes(m_camModel, m_sync);
    }

    m_rpassGBuff->EndRenderpass(m_sync);

    m_rpassLight->BeginRenderpass(m_sync);
    m_plineLight->Bind(m_sync);
    m_plineLight->DrawIndexedVertexBuffer(m_renderSurface->GetVertexBuffer(), m_sync);
    m_plineLight->Unbind(m_sync);
    m_rpassLight->EndRenderpass(m_sync);

    m_sync->Stop();
}

void RD_RenderingPipeline_PBR::Resize(int w, int h) {
    m_rpassGBuff->SetRenderpassSize(m_api.get(), w, h);
    m_rpassLight->SetRenderpassSize(m_api.get(), w, h);
    m_api->GetWindowingSystem()->SetPresentTexture(m_rpassLight->GetAttachment(0));

    for(auto& p : m_pline_refs) {
        p->RebuildPipeline();
    }

    m_plineLight->PurgeTextures();
    for(int i = 0; i < 5; i++) {
        m_plineLight->RegisterTexture(m_rpassGBuff->GetAttachment(i), i + 10);
    }

    std::vector<std::shared_ptr<RD_Texture>> tArray = {};
    for(auto& s : m_depthFBs) {
        tArray.push_back(s->GetAttachment(0));
    }

    m_plineLight->RegisterTextureArray(tArray, 20);

    m_plineLight->RebuildPipeline();

    vec2 scrSize = vec2(w, h);
    m_camData->PartialFillBufferData(scrSize.GetData(), 2 * sizeof(float), 4 * 2 * sizeof(float));
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

void RD_RenderingPipeline_PBR::SetupPipeline(std::shared_ptr<RD_Pipeline> pline) {
    pline->RegisterUniformBuffer(m_camModel);
    pline->RegisterUniformBuffer(m_bonesBuffer);

    m_pline_refs.push_back(pline);
}

void RD_RenderingPipeline_PBR::RenderShadows(
        std::vector<std::shared_ptr<RD_Material>>& sceneData,
        std::vector<std::shared_ptr<RD_DirLight>>& lightData
) {
    int scaster_idx = 0;
    for(uint32_t i = 0; i < lightData.size(); i++) {
        if(!lightData[i]->IsShadowCaster()) {
            continue;
        }

        m_sync->Start();
        m_rpassShadowDepth->BeginRenderpass(m_sync);
        m_plineShadowDepth->Bind(m_sync);

        m_plineShadowDepth->PartialPushConstant(&scaster_idx, sizeof(uint32_t), 16 * sizeof(float), m_sync);
        scaster_idx++;

        for (auto &sd: sceneData) {
            sd->RenderMeshesExtPline(m_plineShadowDepth, m_sync);
        }

        m_plineShadowDepth->Unbind(m_sync);
        m_rpassShadowDepth->EndRenderpass(m_sync);

        //BLUR SHADOWS
        vec2 horiz = vec2(1, 0);
        vec2 vert = vec2(0, 1);

        m_rpassSblur->BeginRenderpassExt(m_sync, m_sblur);
        m_plineSblur->Bind(m_sync);

        m_plineSblur->PushConstant(horiz.GetData(), 2 * sizeof(float), m_sync);
        m_plineSblur->DrawIndexedVertexBuffer(m_renderSurface->GetVertexBuffer(), m_sync);

        m_plineSblur->Unbind(m_sync);
        m_rpassSblur->EndRenderpassEXT(m_sync, m_sblur);

        m_rpassSblur->BeginRenderpassExt(m_sync, m_depthFBs[i]);
        m_plineSblur_b->Bind(m_sync);

        m_plineSblur_b->PushConstant(vert.GetData(), 2 * sizeof(float), m_sync);
        m_plineSblur_b->DrawIndexedVertexBuffer(m_renderSurface->GetVertexBuffer(), m_sync);

        m_plineSblur_b->Unbind(m_sync);
        m_rpassSblur->EndRenderpassEXT(m_sync, m_depthFBs[i]);

        m_sync->Stop();
    }
}

void RD_RenderingPipeline_PBR::SetNumberOfShadowFB(int nbr) {
    RD_Attachment monocolorf{};
    monocolorf.format = IMGFORMAT_R32F;
    monocolorf.do_clear = true;
    monocolorf.is_swapchain_attachment = false;
    monocolorf.sample_count = 1;

    RD_Attachment depth{};
    depth.format = IMGFORMAT_DEPTH;
    depth.do_clear = true;
    depth.is_swapchain_attachment = false;
    depth.sample_count = 1;

    if(nbr < m_depthFBs.size()) {
        //Removing FBs in excess

        m_depthFBs.erase(m_depthFBs.begin() + nbr, m_depthFBs.end());
    } else if(nbr > m_depthFBs.size()) {
        // Creating missing FBs
        int missingFBNbrs = nbr - m_depthFBs.size();

        for(int i = 0; i < missingFBNbrs; i++) {
            std::shared_ptr<RD_OrphanFramebuffer> fb = m_api->CreateOrphanFramebuffer(m_rpassShadowDepth, {depth, monocolorf}, SHADOW_RES, SHADOW_RES);
            fb->BuildFramebuffer(m_api.get());

            m_depthFBs.push_back(fb);
        }
    }

    UpdateShadowTexArray();
}

void RD_RenderingPipeline_PBR::UpdateShadowTexArray() {
    std::vector<std::shared_ptr<RD_Texture>> tArray;
    for(auto& s : m_depthFBs) {
        tArray.push_back(s->GetAttachment(1));
    }

    m_plineLight->SetTextureArray(tArray, 20);
    m_plineLight->RebuildPipeline();
}

void RD_RenderingPipeline_PBR::PushLightMat(mat4f mat, int idx) {
    m_lightMat->PartialFillBufferData(mat.GetPTR(), 16 * sizeof(float), idx * 16 * sizeof(float));
}

void RD_RenderingPipeline_PBR::SetupSkeleton(std::shared_ptr<RD_Skeleton> skel) {
    skel->UploadSkeleton(m_bonesBuffer);
}