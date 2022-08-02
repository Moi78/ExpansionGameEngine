#include "RD_Pipeline.h"

#ifdef BUILD_VULKAN

RD_Pipeline_Vk::RD_Pipeline_Vk (VkDevice dev, VkCommandPool pool, std::shared_ptr< RD_RenderPass > rpass, std::shared_ptr< RD_ShaderLoader > shader) {
    m_dev = dev;
    m_pool = pool;

    m_rpass = std::reinterpret_pointer_cast<RD_RenderPass_Vk>(rpass);
    m_shader = std::reinterpret_pointer_cast<RD_ShaderLoader_Vk>(shader);
}

RD_Pipeline_Vk::~RD_Pipeline_Vk() {
    CleanUp();
}

void RD_Pipeline_Vk::CleanUp() {
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

    VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
    colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendInfo.logicOpEnable = VK_FALSE;
    colorBlendInfo.attachmentCount = 1;
    colorBlendInfo.pAttachments = &colorBlendState;

    //PIPELINE LAYOUT
    VkPipelineLayoutCreateInfo plineLayoutInfo{};
    plineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

    if(vkCreatePipelineLayout(m_dev, &plineLayoutInfo, nullptr, &m_layout) != VK_SUCCESS) {
        std::cerr << "Failed to create pipeline layout." << std::endl;
        return false;
    }

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
    plineInfo.pDepthStencilState = nullptr;
    plineInfo.pColorBlendState = &colorBlendInfo;
    plineInfo.pDynamicState = nullptr;
    plineInfo.layout = m_layout;
    plineInfo.renderPass = m_rpass->GetRenderPassHandle();
    plineInfo.subpass = 0;
    plineInfo.basePipelineHandle = VK_NULL_HANDLE;
    plineInfo.basePipelineIndex = -1;

    if(vkCreateGraphicsPipelines(m_dev, VK_NULL_HANDLE, 1, &plineInfo, nullptr, &m_pipeline) != VK_SUCCESS) {
        std::cerr << "Failed to create a graphic pipeline." << std::endl;
        return false;
    }

    return AllocCMDBuffer();
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

void RD_Pipeline_Vk::Bind() {
    VkCommandBufferBeginInfo bInfo{};
    bInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    vkBeginCommandBuffer(m_cmdBuffer, &bInfo);
    m_rpass->BeginRenderPass(m_cmdBuffer);
    vkCmdBindPipeline(m_cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
}

void RD_Pipeline_Vk::BindSC(VkFramebuffer fb) {
    VkCommandBufferBeginInfo bInfo{};
    bInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    vkResetCommandBuffer(m_cmdBuffer, 0);

    vkBeginCommandBuffer(m_cmdBuffer, &bInfo);
    m_rpass->BeginRenderPass(m_cmdBuffer, fb);
    vkCmdBindPipeline(m_cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
}


void RD_Pipeline_Vk::Unbind() {
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
        std::cerr << "Failed to submit queue" << std::endl;
    }
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

#endif //BUILD_VULKAN
