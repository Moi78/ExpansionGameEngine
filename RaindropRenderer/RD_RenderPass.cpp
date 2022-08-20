#include "RD_RenderPass.h"
#include "RD_API.h"

#ifdef BUILD_VULKAN

RD_RenderPass_Vk::RD_RenderPass_Vk(VkDevice dev, std::vector<RD_Attachment> attachments, float width, float height) {
    m_dev = dev;
    m_w = width;
    m_h = height;

    m_fb = VK_NULL_HANDLE;

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
        desc.format = GetVKFormat(att.format);
        desc.loadOp = att.do_clear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        desc.samples = map_sample[att.sample_count];
        desc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        desc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        desc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        desc.finalLayout = att.is_swapchain_attachment ? VK_IMAGE_LAYOUT_PRESENT_SRC_KHR : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        m_att.push_back(desc);
    }

    m_att_desc = attachments;
}

RD_RenderPass_Vk::~RD_RenderPass_Vk() {
    vkDestroyFramebuffer(m_dev, m_fb, nullptr);
    vkDestroyRenderPass(m_dev, m_renderPass, nullptr);
}

bool RD_RenderPass_Vk::BuildRenderpass(RD_API* api, bool sc) {
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

    if(sc) {
        VkSubpassDependency dep{};
        dep.srcSubpass = VK_SUBPASS_EXTERNAL;
        dep.dstSubpass = 0;
        dep.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dep.srcAccessMask = 0;
        dep.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dep.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        deps.emplace_back(dep);
    } else {
        VkSubpassDependency dep_{};
        dep_.srcSubpass = VK_SUBPASS_EXTERNAL;
        dep_.dstSubpass = 0;
        dep_.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dep_.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        dep_.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        dep_.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

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

    if(!sc) {
        if (!MakeFramebuffer(api)) {
            return false;
        }
    }

    return true;
}

void RD_RenderPass_Vk::BeginRenderPass(VkCommandBuffer cmd, VkFramebuffer scFB) {
    for(auto& img : m_imgs) {
        auto imgVK = std::reinterpret_pointer_cast<RD_Texture_Vk>(img);
        imgVK->PrepareForRendering(cmd);
    }

    VkRenderPassBeginInfo rpassInfo{};
    rpassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rpassInfo.renderPass = m_renderPass;

    if(scFB != VK_NULL_HANDLE) {
        rpassInfo.framebuffer = scFB; // TO COMPLETE
    } else {
        rpassInfo.framebuffer = m_fb;
    }

    rpassInfo.renderArea.offset = {0, 0};
    rpassInfo.renderArea.extent = {(uint32_t)m_w, (uint32_t)m_h};

    constexpr VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    std::vector<VkClearValue> clear;
    for(auto& a : m_att) {
        clear.push_back(clearColor);
    }

    rpassInfo.clearValueCount = clear.size();
    rpassInfo.pClearValues = clear.data();

    vkCmdBeginRenderPass(cmd, &rpassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void RD_RenderPass_Vk::EndRenderPass(VkCommandBuffer cmd) {
    vkCmdEndRenderPass(cmd);

    for(auto& img : m_imgs) {
        auto imgVK = std::reinterpret_pointer_cast<RD_Texture_Vk>(img);
        imgVK->PrepareForSampling(cmd);
    }
}

void RD_RenderPass_Vk::SetRenderpassSize(const int w, const int h) {
    m_w = w;
    m_h = h;
}

bool RD_RenderPass_Vk::MakeFramebuffer(RD_API* api) {
    std::vector<VkImageView> imgViews;
    for(auto& a : m_att_desc) {
        auto img = std::reinterpret_pointer_cast<RD_Texture_Vk>(api->CreateTexture());
        img->CreateTextureFBReady(a.format, m_w, m_h);

        m_imgs.push_back(img);
        imgViews.push_back(img->GetView());
    }

    VkFramebufferCreateInfo cInfo{};
    cInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    cInfo.width = static_cast<uint32_t>(m_w);
    cInfo.height = static_cast<uint32_t>(m_h);
    cInfo.layers = 1;
    cInfo.attachmentCount = m_att.size();
    cInfo.pAttachments = imgViews.data();
    cInfo.renderPass = m_renderPass;

    if(vkCreateFramebuffer(m_dev, &cInfo, nullptr, &m_fb) != VK_SUCCESS) {
        std::cerr << "ERROR: Failed to create framebuffer." << std::endl;
        return false;
    }

    return true;
}

std::shared_ptr<RD_Texture> RD_RenderPass_Vk::GetAttachment(int index) {
    assert(index < m_imgs.size() && "Bad index given");

    return m_imgs[index];
}

#endif //BUILD_VULKAN
