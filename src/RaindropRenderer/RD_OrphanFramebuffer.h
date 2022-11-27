#ifndef EXPGE_RD_ORPHANFRAMEBUFFER_H
#define EXPGE_RD_ORPHANFRAMEBUFFER_H

#include <memory>

#include "RD_Texture.h"

class RD_API;

class RD_OrphanFramebuffer {
public:
    RD_OrphanFramebuffer() {};
    ~RD_OrphanFramebuffer() {};

    virtual bool BuildFramebuffer(RD_API* api) = 0;

    virtual std::shared_ptr<RD_Texture> GetAttachment(int idx) = 0;
};

#ifdef BUILD_VULKAN

#include <unordered_map>
#include <vulkan/vulkan.hpp>

#include "RD_RenderPass.h"

class RD_OrphanFramebuffer_Vk : public RD_OrphanFramebuffer {
public:
    RD_OrphanFramebuffer_Vk(VkDevice dev, std::shared_ptr<RD_RenderPass> rpass, std::vector<RD_Attachment> attachments, float width, float height);
    ~RD_OrphanFramebuffer_Vk();

    bool BuildFramebuffer(RD_API* api) override;
    VkFramebuffer GetFramebufferHandle() { return m_fb; }

    void PrepareForSampling(VkCommandBuffer cmd);
    void PrepareForRendering(VkCommandBuffer cmd);

    std::shared_ptr<RD_Texture> GetAttachment(int idx) override;
private:
    VkDevice m_dev;
    std::shared_ptr<RD_RenderPass> m_rpass;

    VkFramebuffer m_fb;
    std::vector<std::shared_ptr<RD_Texture_Vk>> m_imgs;

    std::vector<VkAttachmentDescription> m_att;
    std::vector<RD_Attachment> m_att_desc;

    float m_w, m_h;
};

#endif //BUILD_VULKAN

#endif //EXPGE_RD_ORPHANFRAMEBUFFER_H
