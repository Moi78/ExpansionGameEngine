#include "RD_RenderPass.h"

#ifdef BUILD_VULKAN

RD_RenderPass_Vk::RD_RenderPass_Vk(VkDevice dev, std::vector<RD_Attachment> attachments, float width, float height) {
    m_dev = dev;
    m_w = width;
    m_h = height;

    std::map<int, VkFormat> map_formats {
        {IMGFORMAT_R, VK_FORMAT_R8_SRGB},
        {IMGFORMAT_RG, VK_FORMAT_R8G8_SRGB},
        {IMGFORMAT_RGB, VK_FORMAT_R8G8B8_SRGB},
        {IMGFORMAT_RGBA, VK_FORMAT_R8G8B8A8_SRGB},

        {IMGFORMAT_R16F, VK_FORMAT_R32_SFLOAT},
        {IMGFORMAT_RG16F, VK_FORMAT_R32G32_SFLOAT},
        {IMGFORMAT_RGB16F, VK_FORMAT_R32G32B32_SFLOAT},
        {IMGFORMAT_RGBA16F, VK_FORMAT_R32G32B32A32_SFLOAT},

        {IMGFORMAT_DEPTH, VK_FORMAT_D32_SFLOAT},

        {IMGFORMAT_BGRA, VK_FORMAT_B8G8R8A8_UNORM},
        {IMGFORMAT_BGR, VK_FORMAT_B8G8R8_UNORM}
    };

    std::map<int, VkSampleCountFlagBits> map_sample {
        {1, VK_SAMPLE_COUNT_1_BIT},
        {2, VK_SAMPLE_COUNT_2_BIT},
        {4, VK_SAMPLE_COUNT_4_BIT},
        {8, VK_SAMPLE_COUNT_8_BIT},
        {16, VK_SAMPLE_COUNT_16_BIT},
        {32, VK_SAMPLE_COUNT_32_BIT},
        {64, VK_SAMPLE_COUNT_64_BIT}
    };

    for(auto& att : attachments) {
        assert(att.sample_count != 0 && ((att.sample_count & (att.sample_count - 1)) == 0) && (att.sample_count <= 64) && "Sample count must be a power of 2 and between 0 and 64.");

        VkAttachmentDescription desc{};
        desc.format = map_formats[att.format];
        desc.loadOp = att.do_clear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        desc.samples = map_sample[att.sample_count];
        desc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        desc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        desc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        desc.finalLayout = att.is_swapchain_attachment ? VK_IMAGE_LAYOUT_PRESENT_SRC_KHR : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        m_att.push_back(desc);
    }
}

RD_RenderPass_Vk::~RD_RenderPass_Vk() {
    vkDestroyRenderPass(m_dev, m_renderPass, nullptr);
}

bool RD_RenderPass_Vk::BuildRenderpass(bool sc) {
    std::vector<VkAttachmentReference> refs(m_att.size());

    for(int i = 0; i < m_att.size(); i++) {
        VkAttachmentReference attRef{};
        attRef.attachment = i;
        attRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        refs[i] = attRef;
    }

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = refs.size();
    subpass.pColorAttachments = refs.data();

    std::vector<VkSubpassDependency> deps;
    VkSubpassDependency dep{};
    dep.srcSubpass = VK_SUBPASS_EXTERNAL;
    dep.dstSubpass = 0;
    dep.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dep.srcAccessMask = 0;
    dep.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dep.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    deps.emplace_back(dep);

    if(!sc) {
        VkSubpassDependency dep_{};
        dep_.srcSubpass = 0;
        dep_.dstSubpass = VK_SUBPASS_EXTERNAL;
        dep_.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dep_.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        dep_.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        dep_.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
        dep_.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

        deps.emplace_back(dep_);
    }

    VkRenderPassCreateInfo cInfo{};
    cInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    cInfo.attachmentCount = m_att.size();
    cInfo.pAttachments = m_att.data();
    cInfo.subpassCount = 1;
    cInfo.pSubpasses = &subpass;
    cInfo.dependencyCount = static_cast<uint32_t>(deps.size());
    cInfo.pDependencies = deps.data();

    if(vkCreateRenderPass(m_dev, &cInfo, nullptr, &m_renderPass) != VK_SUCCESS) {
        std::cerr << "Failed to build render pass." << std::endl;
        return false;
    }

    if(!CreateImageViews()) {
        return false;
    }

    if(!MakeFramebuffers()) {
        return false;
    }

    return true;
}

void RD_RenderPass_Vk::BeginRenderPass(VkCommandBuffer cmd, VkFramebuffer scFB) {
    VkRenderPassBeginInfo rpassInfo{};
    rpassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rpassInfo.renderPass = m_renderPass;

    if(scFB != VK_NULL_HANDLE) {
        rpassInfo.framebuffer = scFB; // TO COMPLETE
    }

    rpassInfo.renderArea.offset = {0, 0};
    rpassInfo.renderArea.extent = {(uint32_t)m_w, (uint32_t)m_h};

    constexpr VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    rpassInfo.clearValueCount = 1;
    rpassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(cmd, &rpassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void RD_RenderPass_Vk::EndRenderPass(VkCommandBuffer cmd) {
    vkCmdEndRenderPass(cmd);
}

void RD_RenderPass_Vk::SetRenderpassSize(const int w, const int h) {
    m_w = w;
    m_h = h;
}

bool RD_RenderPass_Vk::CreateImageViews() {

    return true;
}

bool RD_RenderPass_Vk::MakeFramebuffers() {

    return true;
}

#endif //BUILD_VULKAN
