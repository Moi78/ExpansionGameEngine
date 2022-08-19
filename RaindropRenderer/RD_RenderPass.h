#ifndef _RD_RENDERPASS_H__
#define _RD_RENDERPASS_H__

#include "RD_Texture.h"
#include "RD_ImageFormat.h"

#include <iostream>
#include <vector>
#include <map>

struct RD_Attachment {
    int format;
    int sample_count;
    bool do_clear;

    bool is_swapchain_attachment = false;
};

class RD_RenderPass {
public:
    RD_RenderPass() {};
    virtual ~RD_RenderPass() {};

    virtual bool BuildRenderpass(bool sc) = 0;
    virtual void SetRenderpassSize(const int w, const int h) = 0;

    virtual int GetAttachmentCount() = 0;
};

#ifdef BUILD_VULKAN

#include <vulkan/vulkan.hpp>

class RD_API;
class RD_API_Vk;

class RD_RenderPass_Vk : public RD_RenderPass {
public:
    RD_RenderPass_Vk(std::shared_ptr<RD_API> api, VkDevice dev, std::vector<RD_Attachment> attachments, float width, float height);
    ~RD_RenderPass_Vk() override;

    void BeginRenderPass(VkCommandBuffer cmd, VkFramebuffer scFB = VK_NULL_HANDLE);
    void EndRenderPass(VkCommandBuffer cmd);

    bool BuildRenderpass(bool sc) override;
    void SetRenderpassSize(const int w, const int h) override;

    float GetWidth() {
        return m_w;
    }

    float GetHeight() {
        return m_h;
    }

    VkRenderPass GetRenderPassHandle() {
        return m_renderPass;
    }

    int GetAttachmentCount() override {
        return m_att_desc.size();
    }

private:
    bool MakeFramebuffer();

    std::shared_ptr<RD_API_Vk> m_api;

    VkFramebuffer m_fb;
    std::vector<std::shared_ptr<RD_Texture_Vk>> m_images;
    std::vector<std::shared_ptr<RD_Texture_Vk>> m_imgs;

    VkDevice m_dev;
    VkRenderPass m_renderPass;

    std::vector<VkAttachmentDescription> m_att;
    std::vector<RD_Attachment> m_att_desc;
    float m_w, m_h;
};

#endif //BUILD_VULKAN

#endif //_RD_RENDERPASS_H__
