#ifndef _RD_RENDERPASS_H__
#define _RD_RENDERPASS_H__

#include <iostream>
#include <vector>
#include <map>

#define IMGFORMAT_R 1
#define IMGFORMAT_RG 2
#define IMGFORMAT_RGB 3
#define IMGFORMAT_RGBA 4

#define IMGFORMAT_R16F 5
#define IMGFORMAT_RG16F 6
#define IMGFORMAT_RGB16F 7
#define IMGFORMAT_RGBA16F 8

#define IMGFORMAT_DEPTH 9

#define IMGFORMAT_BGRA 10
#define IMGFORMAT_BGR 11

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
};

#ifdef BUILD_VULKAN

#include <vulkan/vulkan.hpp>

class RD_RenderPass_Vk : public RD_RenderPass {
public:
    RD_RenderPass_Vk(VkDevice dev, std::vector<RD_Attachment> attachments, float width, float height);
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

private:
    bool CreateImageViews();
    bool MakeFramebuffers();

    VkFramebuffer m_fb;
    std::vector<VkImageView> m_views;
    std::vector<VkImage> m_images;

    VkDevice m_dev;
    VkRenderPass m_renderPass;

    std::vector<VkAttachmentDescription> m_att;
    float m_w, m_h;
};

#endif //BUILD_VULKAN

#endif //_RD_RENDERPASS_H__
