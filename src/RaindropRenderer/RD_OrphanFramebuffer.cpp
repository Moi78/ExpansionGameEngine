#include "RD_OrphanFramebuffer.h"
#include "RD_API.h"

#ifdef BUILD_VULKAN

RD_OrphanFramebuffer_Vk::RD_OrphanFramebuffer_Vk(VkDevice dev, std::shared_ptr<RD_RenderPass> rpass, std::vector<RD_Attachment> attachements, float w, float h) {
    m_dev = dev;
    m_rpass = rpass;

    m_w = w;
    m_h = h;

    std::unordered_map<int, VkSampleCountFlagBits> map_sample {
            {1, VK_SAMPLE_COUNT_1_BIT},
            {2, VK_SAMPLE_COUNT_2_BIT},
            {4, VK_SAMPLE_COUNT_4_BIT},
            {8, VK_SAMPLE_COUNT_8_BIT},
            {16, VK_SAMPLE_COUNT_16_BIT},
            {32, VK_SAMPLE_COUNT_32_BIT},
            {64, VK_SAMPLE_COUNT_64_BIT}
    };

    m_att_desc = attachements;
    for(auto& att : m_att_desc) {
        assert(att.sample_count != 0 && ((att.sample_count & (att.sample_count - 1)) == 0) && (att.sample_count <= 64) && "Sample count must be a power of 2 and between 0 and 64.");

        VkAttachmentDescription desc{};
        desc.format = GetVKFormat(att.format);
        desc.loadOp = att.do_clear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        desc.samples = map_sample[att.sample_count];
        desc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        desc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        desc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        if(desc.format == VK_FORMAT_D32_SFLOAT) {
            desc.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        } else {
            desc.finalLayout = att.is_swapchain_attachment ? VK_IMAGE_LAYOUT_PRESENT_SRC_KHR : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        }

        m_att.push_back(desc);
    }
}

RD_OrphanFramebuffer_Vk::~RD_OrphanFramebuffer_Vk() {
    vkDestroyFramebuffer(m_dev, m_fb, nullptr);
}

bool RD_OrphanFramebuffer_Vk::BuildFramebuffer(RD_API* api) {
    std::vector<VkImageView> imgViews;
    for(auto& a : m_att_desc) {
        auto img = std::reinterpret_pointer_cast<RD_Texture_Vk>(api->CreateTexture());
        img->CreateTextureFBReady(a.format, m_w, m_h);

        m_imgs.push_back(img);
        imgViews.push_back(img->GetView());
    }

    auto rpassVK = std::reinterpret_pointer_cast<RD_RenderPass_Vk>(m_rpass);

    VkFramebufferCreateInfo cInfo{};
    cInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    cInfo.width = static_cast<uint32_t>(m_w);
    cInfo.height = static_cast<uint32_t>(m_h);
    cInfo.layers = 1;
    cInfo.attachmentCount = m_att.size();
    cInfo.pAttachments = imgViews.data();
    cInfo.renderPass = rpassVK->GetRenderPassHandle();

    if(vkCreateFramebuffer(m_dev, &cInfo, nullptr, &m_fb) != VK_SUCCESS) {
        std::cerr << "ERROR: Failed to create framebuffer." << std::endl;
        return false;
    }

    return true;
}

void RD_OrphanFramebuffer_Vk::PrepareForSampling(VkCommandBuffer cmd) {
    for(auto& img : m_imgs) {
        auto imgVK = std::reinterpret_pointer_cast<RD_Texture_Vk>(img);
        img->PrepareForSampling(cmd);
    }
}

void RD_OrphanFramebuffer_Vk::PrepareForRendering(VkCommandBuffer cmd) {
    for(auto& img : m_imgs) {
        auto imgVK = std::reinterpret_pointer_cast<RD_Texture_Vk>(img);
        img->PrepareForRendering(cmd);
    }
}

std::shared_ptr<RD_Texture> RD_OrphanFramebuffer_Vk::GetAttachment(int idx) {
    assert(idx < m_imgs.size() && idx >= 0);

    return m_imgs[idx];
}

#endif //BUILD_VULKAN