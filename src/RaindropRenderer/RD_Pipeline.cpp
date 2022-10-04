#include "RD_Pipeline.h"

#ifdef BUILD_VULKAN

RD_Pipeline_Vk::RD_Pipeline_Vk (VkDevice dev, VkCommandPool pool, VkQueue gfxQueue, std::shared_ptr< RD_RenderPass > rpass, std::shared_ptr< RD_ShaderLoader > shader, bool extSignaling) {
    m_dev = dev;
    m_pool = pool;
    m_gfxQueue = gfxQueue;

    m_extSignaling = extSignaling;
    m_isModelMode = false;

    m_rpass = std::reinterpret_pointer_cast<RD_RenderPass_Vk>(rpass);
    m_shader = std::reinterpret_pointer_cast<RD_ShaderLoader_Vk>(shader);

    m_descSet = VK_NULL_HANDLE;
    m_descLayout = VK_NULL_HANDLE;
    m_descPool = VK_NULL_HANDLE;

    m_cmdBuffer = VK_NULL_HANDLE;
    m_layout = VK_NULL_HANDLE;
    m_pipeline = VK_NULL_HANDLE;

    m_fence = VK_NULL_HANDLE;
}

RD_Pipeline_Vk::~RD_Pipeline_Vk() {
    CleanUp();
}

void RD_Pipeline_Vk::CleanUp() {
    vkDestroyFence(m_dev, m_fence, nullptr);
    vkDestroyDescriptorSetLayout(m_dev, m_descLayout, nullptr);
    vkDestroyDescriptorPool(m_dev, m_descPool, nullptr);
    vkDestroyPipelineLayout(m_dev, m_layout, nullptr);
    vkDestroyPipeline(m_dev, m_pipeline, nullptr);
}

bool RD_Pipeline_Vk::BuildPipeline() {
    // SHADER STAGES
    VkPipelineShaderStageCreateInfo vcInfo{};
    vcInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vcInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vcInfo.module = m_shader->GetVertShaderModule();
    vcInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fcInfo{};
    fcInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fcInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fcInfo.module = m_shader->GetFragShaderModule();
    fcInfo.pName = "main";

    const std::array<VkPipelineShaderStageCreateInfo, 2> stages = {vcInfo, fcInfo};

    // FIXED FUNCTIONS
    std::array<VkVertexInputAttributeDescription, 3> attrs = {};

    //POSITION
    attrs[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attrs[0].binding = 0;
    attrs[0].location = 0;
    attrs[0].offset = 0;

    //NORMAL
    attrs[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attrs[1].binding = 0;
    attrs[1].location = 1;
    attrs[1].offset = sizeof(float) * 3;

    //UV
    attrs[2].format = VK_FORMAT_R32G32_SFLOAT;
    attrs[2].binding = 0;
    attrs[2].location = 2;
    attrs[2].offset = sizeof(float) * 6;

    VkVertexInputBindingDescription bindDesc{};
    bindDesc.binding = 0;
    bindDesc.stride = sizeof(float) * 8;
    bindDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    VkPipelineVertexInputStateCreateInfo vinInfo{};
    vinInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vinInfo.vertexBindingDescriptionCount = 1;
    vinInfo.pVertexBindingDescriptions = &bindDesc;
    vinInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attrs.size());
    vinInfo.pVertexAttributeDescriptions = attrs.data();

    VkPipelineInputAssemblyStateCreateInfo asmInfo{};
    asmInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    asmInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    asmInfo.primitiveRestartEnable = VK_FALSE;

    VkViewport vp{};
    vp.x = 0.0f;
    vp.y = 0.0f;
    vp.width = m_rpass->GetWidth();
    vp.height = m_rpass->GetHeight();
    vp.minDepth = 0.0f;
    vp.maxDepth = 1.0f;

    VkRect2D scissors{};
    scissors.offset = {0, 0};
    scissors.extent = {(uint32_t)vp.width, (uint32_t)vp.height};

    VkPipelineViewportStateCreateInfo vpStInfo{};
    vpStInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    vpStInfo.viewportCount = 1;
    vpStInfo.pViewports = &vp;
    vpStInfo.scissorCount = 1;
    vpStInfo.pScissors = &scissors;

    VkPipelineRasterizationStateCreateInfo rasterizerInfo{};
    rasterizerInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizerInfo.depthClampEnable = VK_FALSE;
    rasterizerInfo.rasterizerDiscardEnable = VK_FALSE;
    rasterizerInfo.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizerInfo.lineWidth = 1.0f;
    rasterizerInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizerInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizerInfo.depthBiasEnable = VK_FALSE;
    rasterizerInfo.depthBiasConstantFactor = 0.0f;
    rasterizerInfo.depthBiasClamp = 0.0f;
    rasterizerInfo.depthBiasSlopeFactor = 0.0f;

    VkPipelineMultisampleStateCreateInfo ms{};
    ms.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    ms.sampleShadingEnable = VK_FALSE;
    ms.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    ms.minSampleShading = 1.0f;
    ms.pSampleMask = nullptr;
    ms.alphaToCoverageEnable = VK_FALSE;
    ms.alphaToOneEnable = VK_FALSE;

    VkPipelineColorBlendAttachmentState colorBlendState{};
    colorBlendState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
                                     VK_COLOR_COMPONENT_G_BIT |
                                     VK_COLOR_COMPONENT_B_BIT |
                                     VK_COLOR_COMPONENT_A_BIT;
    colorBlendState.blendEnable = VK_FALSE;

    std::vector<VkPipelineColorBlendAttachmentState> blendAtt;
    for(int i = 0; i < m_rpass->GetAttachmentCount(); i++) {
        blendAtt.push_back(colorBlendState);
    }

    if(m_rpass->HasDepth()) {
        blendAtt.pop_back();
    }

    VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
    colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendInfo.logicOpEnable = VK_FALSE;
    colorBlendInfo.attachmentCount = blendAtt.size();
    colorBlendInfo.pAttachments = blendAtt.data();

    //PIPELINE LAYOUT
    VkPipelineLayoutCreateInfo plineLayoutInfo{};
    plineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

    if(m_isModelMode) {
        VkPushConstantRange push_constant{};
        push_constant.size = 16 * sizeof(float);
        push_constant.offset = 0;
        push_constant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        plineLayoutInfo.pPushConstantRanges = &push_constant;
        plineLayoutInfo.pushConstantRangeCount = 1;
    }

    //UNIFORM BUFFERS // TEXTURE BUFFERS (If any)
    if((!m_uBuffs.empty()) || (!m_texs.empty())) {
        std::vector<VkDescriptorSetLayoutBinding> bindings;
        bindings.insert(bindings.begin(), m_bindings.begin(), m_bindings.end());
        bindings.insert(bindings.end(), m_bindings_tex.begin(), m_bindings_tex.end());

        VkDescriptorSetLayoutCreateInfo bindingLayoutInfo{};
        bindingLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        bindingLayoutInfo.bindingCount = bindings.size();
        bindingLayoutInfo.pBindings = bindings.data();

        if(vkCreateDescriptorSetLayout(m_dev, &bindingLayoutInfo, nullptr, &m_descLayout) != VK_SUCCESS) {
            std::cerr << "ERROR: Failed to create descriptor set layout." << std::endl;
        }

        plineLayoutInfo.setLayoutCount = 1;
        plineLayoutInfo.pSetLayouts = &m_descLayout;

        if(CreateDescriptorPool()) {
            CreateDescriptorSet();
        }
    }

    if(vkCreatePipelineLayout(m_dev, &plineLayoutInfo, nullptr, &m_layout) != VK_SUCCESS) {
        std::cerr << "Failed to create pipeline layout." << std::endl;
        return false;
    }

    VkPipelineDepthStencilStateCreateInfo depthInfo{};
    depthInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthInfo.depthTestEnable = VK_TRUE;
    depthInfo.depthWriteEnable = VK_TRUE;
    depthInfo.depthCompareOp = VK_COMPARE_OP_LESS;
    depthInfo.depthBoundsTestEnable = VK_FALSE;
    depthInfo.stencilTestEnable = VK_FALSE;

    //PIPELINE CREATION
    VkGraphicsPipelineCreateInfo plineInfo{};
    plineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    plineInfo.stageCount = 2;
    plineInfo.pStages = stages.data();
    plineInfo.pVertexInputState = &vinInfo;
    plineInfo.pInputAssemblyState = &asmInfo;
    plineInfo.pViewportState = &vpStInfo;
    plineInfo.pRasterizationState = &rasterizerInfo;
    plineInfo.pMultisampleState = &ms;
    plineInfo.pDepthStencilState = &depthInfo;
    plineInfo.pColorBlendState = &colorBlendInfo;
    plineInfo.pDynamicState = nullptr;
    plineInfo.layout = m_layout;
    plineInfo.renderPass = m_rpass->GetRenderPassHandle();
    plineInfo.subpass = 0;
    plineInfo.basePipelineHandle = VK_NULL_HANDLE;
    plineInfo.basePipelineIndex = -1;

    if(vkCreateGraphicsPipelines(m_dev, VK_NULL_HANDLE, 1, &plineInfo, nullptr, &m_pipeline) != VK_SUCCESS) {
        std::cerr << "ERROR: Failed to create a graphic pipeline." << std::endl;
        return false;
    }

    if(!m_extSignaling) {
        if(!BuildSyncObjects()) {
            std::cerr << "ERROR: Failed to create sync objects for pipeline." << std::endl;
            return false;
        }
    }

    return AllocCMDBuffer();
}

bool RD_Pipeline_Vk::CreateDescriptorPool() {
    std::vector<VkDescriptorPoolSize> psize;

    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = m_bindings.size();

    if(poolSize.descriptorCount > 0) {
        psize.push_back(poolSize);
    }

    VkDescriptorPoolSize poolSize_tex{};
    poolSize_tex.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSize_tex.descriptorCount = m_bindings_tex.size();

    if(poolSize_tex.descriptorCount > 0) {
        psize.push_back(poolSize_tex);
    }

    VkDescriptorPoolCreateInfo cInfo{};
    cInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    cInfo.poolSizeCount = psize.size();
    cInfo.pPoolSizes = psize.data();
    cInfo.maxSets = m_bindings.size() + m_bindings_tex.size();

    if(vkCreateDescriptorPool(m_dev, &cInfo, nullptr, &m_descPool) != VK_SUCCESS) {
        std::cerr << "ERROR: Failed to create descriptor pool." << std::endl;
        return false;
    }

    return true;
}

bool RD_Pipeline_Vk::CreateDescriptorSet() {
    assert(m_descPool != VK_NULL_HANDLE && "Descriptor pool must have been created.");

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = m_descPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &m_descLayout;

    if(vkAllocateDescriptorSets(m_dev, &allocInfo, &m_descSet) != VK_SUCCESS) {
        std::cerr << "ERROR: Failed to allocate descriptor sets." << std::endl;
        return false;
    }

    for(int i = 0; i < m_bindings.size(); i++) {
        std::shared_ptr<RD_UniformBuffer_Vk> vkUBuff = std::reinterpret_pointer_cast<RD_UniformBuffer_Vk>(m_uBuffs[i]);

        VkDescriptorBufferInfo buffInfo{};
        buffInfo.buffer = vkUBuff->GetBufferHandle();
        buffInfo.offset = 0;
        buffInfo.range = vkUBuff->GetBufferSize();

        VkWriteDescriptorSet descW{};
        descW.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descW.dstSet = m_descSet;
        descW.dstBinding = vkUBuff->GetBinding();
        descW.dstArrayElement = 0;
        descW.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descW.descriptorCount = 1;
        descW.pBufferInfo = &buffInfo;
        descW.pImageInfo = nullptr;
        descW.pTexelBufferView = nullptr;

        vkUpdateDescriptorSets(m_dev, 1, &descW, 0, nullptr);
    }

    for(int i = 0; i < m_bindings_tex.size(); i++) {
        std::shared_ptr<RD_Texture_Vk> vkTex = std::reinterpret_pointer_cast<RD_Texture_Vk>(m_texs[i]);

        auto infos = vkTex->GetDescriptorInfo();

        VkWriteDescriptorSet descW{};
        descW.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descW.dstSet = m_descSet;
        descW.dstBinding = m_bindings_tex[i].binding;
        descW.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descW.descriptorCount = 1;
        descW.pBufferInfo = nullptr;
        descW.pImageInfo = &infos;
        descW.pTexelBufferView = nullptr;

        vkUpdateDescriptorSets(m_dev, 1, &descW, 0, nullptr);
    }

    return true;
}

bool RD_Pipeline_Vk::AllocCMDBuffer() {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = m_pool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if(vkAllocateCommandBuffers(m_dev, &allocInfo, &m_cmdBuffer) != VK_SUCCESS) {
        std::cerr << "Failed to allocate a command buffer." << std::endl;
        return false;
    }

    return true;
}

void RD_Pipeline_Vk::Bind(std::optional<std::shared_ptr<RD_RenderSynchronizer>> sync) {
    VkCommandBufferBeginInfo bInfo{};
    bInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    VkCommandBuffer cmdBuff = m_cmdBuffer;

    if(!sync.has_value()) {
        vkResetCommandBuffer(cmdBuff, 0);
        vkBeginCommandBuffer(cmdBuff, &bInfo);
        m_rpass->BeginRenderPass(cmdBuff);
    } else {
        auto syncVK = std::reinterpret_pointer_cast<RD_RenderSynchronizer_Vk>(sync.value());
        cmdBuff = syncVK->GetCommandBuffer();
    }

    vkCmdBindPipeline(cmdBuff, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);

    if(m_descSet != VK_NULL_HANDLE) {
        vkCmdBindDescriptorSets(cmdBuff, VK_PIPELINE_BIND_POINT_GRAPHICS, m_layout, 0, 1, &m_descSet, 0, nullptr);
    }
}

void RD_Pipeline_Vk::BindSC(VkFramebuffer fb) {
    VkCommandBufferBeginInfo bInfo{};
    bInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    vkResetCommandBuffer(m_cmdBuffer, 0);

    vkBeginCommandBuffer(m_cmdBuffer, &bInfo);
    m_rpass->BeginRenderPass(m_cmdBuffer, fb);
    vkCmdBindPipeline(m_cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);

    if(m_descSet != VK_NULL_HANDLE) {
        vkCmdBindDescriptorSets(m_cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_layout, 0, 1, &m_descSet, 0, nullptr);
    }
}

void RD_Pipeline_Vk::Unbind(std::optional<std::shared_ptr<RD_RenderSynchronizer>> sync) {
    if(!sync.has_value()) {
        m_rpass->EndRenderPass(m_cmdBuffer);
        vkEndCommandBuffer(m_cmdBuffer);

        SubmitCMDSync();
    }
}

void RD_Pipeline_Vk::UnbindSC() {
    m_rpass->EndRenderPass(m_cmdBuffer);
    vkEndCommandBuffer(m_cmdBuffer);
}

void RD_Pipeline_Vk::SubmitCMD(VkQueue queue, VkSemaphore waitSignal, VkSemaphore rndrFinished, VkFence inFlight) {
    VkSubmitInfo sbInfo{};
    sbInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSem[] = {waitSignal};
    VkPipelineStageFlags waitStages = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    sbInfo.waitSemaphoreCount = 1;
    sbInfo.pWaitSemaphores = waitSem;
    sbInfo.pWaitDstStageMask = &waitStages;

    VkSemaphore rndrFinished_sem[] = {rndrFinished};
    sbInfo.signalSemaphoreCount = 1;
    sbInfo.pSignalSemaphores = rndrFinished_sem;

    sbInfo.commandBufferCount = 1;
    sbInfo.pCommandBuffers = &m_cmdBuffer;

    if(vkQueueSubmit(queue, 1, &sbInfo, inFlight) != VK_SUCCESS) {
        std::cerr << "ERROR: Failed to submit queue" << std::endl;
    }
}

void RD_Pipeline_Vk::SubmitCMDSync() {
    vkResetFences(m_dev, 1, &m_fence);

    VkSubmitInfo sbInfo{};
    sbInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    sbInfo.waitSemaphoreCount = 0;
    sbInfo.signalSemaphoreCount = 0;

    sbInfo.commandBufferCount = 1;
    sbInfo.pCommandBuffers = &m_cmdBuffer;

    if(vkQueueSubmit(m_gfxQueue, 1, &sbInfo, m_fence) != VK_SUCCESS) {
        std::cerr << "ERROR: Failed to submit queue (sync)." << std::endl;
    }

    vkWaitForFences(m_dev, 1, &m_fence, VK_TRUE, UINT64_MAX);
}

void RD_Pipeline_Vk::RebuildPipeline() {
    CleanUp();
    BuildPipeline();
}

void RD_Pipeline_Vk::DrawVertexBuffer(std::shared_ptr<RD_VertexBuffer> vbuff) {
    const std::shared_ptr<RD_VertexBuffer_Vk> vb = std::reinterpret_pointer_cast<RD_VertexBuffer_Vk>(vbuff);

    vb->BindBuffer(m_cmdBuffer);
    vkCmdDraw(
            m_cmdBuffer, static_cast<uint32_t>(vb->GetBufferSize() / 8),
            1, 0, 0
    );
}

void RD_Pipeline_Vk::DrawIndexedVertexBuffer(std::shared_ptr<RD_IndexedVertexBuffer> vibuff, std::optional<std::shared_ptr<RD_RenderSynchronizer>> sync) {
    const std::shared_ptr<RD_IndexedVertexBuffer_Vk> vib = std::reinterpret_pointer_cast<RD_IndexedVertexBuffer_Vk>(vibuff);

    VkCommandBuffer cmdBuff = m_cmdBuffer;
    if(sync.has_value()) {
        const std::shared_ptr<RD_RenderSynchronizer_Vk> vsync = std::reinterpret_pointer_cast<RD_RenderSynchronizer_Vk>(sync.value());
        cmdBuff = vsync->GetCommandBuffer();
    }

    vib->BindBuffer(cmdBuff);
    vkCmdDrawIndexed(
        cmdBuff,
        vib->GetNumberOfIndices(),
        1, 0, 0, 0
    );
}

void RD_Pipeline_Vk::RegisterUniformBuffer(std::shared_ptr<RD_UniformBuffer>& buff) {
    m_uBuffs.push_back(buff);

    VkDescriptorSetLayoutBinding bindLayout{};
    bindLayout.binding = buff->GetBinding();
    bindLayout.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    bindLayout.descriptorCount = 1;
    bindLayout.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    bindLayout.pImmutableSamplers = nullptr;

    m_bindings.push_back(bindLayout);
}

void RD_Pipeline_Vk::RegisterTexture(std::shared_ptr<RD_Texture> &tex, uint32_t binding) {
    m_texs.push_back(tex);

    VkDescriptorSetLayoutBinding bindLayout{};
    bindLayout.binding = binding;
    bindLayout.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    bindLayout.descriptorCount = 1;
    bindLayout.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    bindLayout.pImmutableSamplers = nullptr;

    m_bindings_tex.push_back(bindLayout);
}

void RD_Pipeline_Vk::PurgeTextures() {
    m_texs.clear();
    m_bindings_tex.clear();
}

bool RD_Pipeline_Vk::BuildSyncObjects() {
    assert(!m_extSignaling && "This instance uses external signaling. No need to build sync objects.");

    VkSemaphoreCreateInfo scInfo{};
    scInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fcInfo{};
    fcInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fcInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    if(vkCreateFence(m_dev, &fcInfo, nullptr, &m_fence) != VK_SUCCESS) {
        std::cerr << "ERROR: Failed to create fence." << std::endl;
        return false;
    }

    return true;
}

void RD_Pipeline_Vk::SetModelMode(bool mode) {
    m_isModelMode = mode;
}

void RD_Pipeline_Vk::PushConstant(void *data, std::optional<std::shared_ptr<RD_RenderSynchronizer>> sync) {
    VkCommandBuffer cmdBuff = m_cmdBuffer;
    if(sync.has_value()) {
        const std::shared_ptr<RD_RenderSynchronizer_Vk> vsync = std::reinterpret_pointer_cast<RD_RenderSynchronizer_Vk>(sync.value());
        cmdBuff = vsync->GetCommandBuffer();
    }

    vkCmdPushConstants(cmdBuff, m_layout, VK_SHADER_STAGE_VERTEX_BIT, 0, 16 * sizeof(float), data);
}

#endif //BUILD_VULKAN
