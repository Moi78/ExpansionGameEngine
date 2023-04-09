#ifndef _RD_RENDERPASS_H__
#define _RD_RENDERPASS_H__

#include "RD_Texture.h"
#include "RD_ImageFormat.h"
#include "RD_RenderSynchronizer.h"

#include "vec.h"

#include <iostream>
#include <vector>
#include <unordered_map>

class RD_API;
class RD_OrphanFramebuffer;

struct RD_Attachment {
    int format;
    int sample_count;

    bool do_clear;
    vec4 clearColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    bool is_swapchain_attachment = false;
    bool is_transparent = false;
};

class RD_RenderPass {
public:
    RD_RenderPass() {};
    virtual ~RD_RenderPass() {};

    virtual bool BuildRenderpass(RD_API* api, bool sc) = 0;
    virtual void SetRenderpassSize(RD_API* api, const int w, const int h) = 0;

    virtual void BeginRenderpass(std::shared_ptr<RD_RenderSynchronizer> sync) = 0;
    virtual void BeginRenderpassExt(std::shared_ptr<RD_RenderSynchronizer> sync, std::shared_ptr<RD_OrphanFramebuffer> fb) = 0;
    virtual void EndRenderpass(std::shared_ptr<RD_RenderSynchronizer> sync) = 0;
    virtual void EndRenderpassEXT(std::shared_ptr<RD_RenderSynchronizer> sync, std::shared_ptr<RD_OrphanFramebuffer> fb) = 0;

    virtual int GetAttachmentCount() = 0;
    virtual std::shared_ptr<RD_Texture> GetAttachment(int index) = 0;

    virtual bool IsAttachmentTransparent(int idx) = 0;

    virtual bool HasDepth() = 0;
};

#ifdef BUILD_VULKAN

#include <vulkan/vulkan.hpp>

class RD_RenderPass_Vk : public RD_RenderPass {
public:
    RD_RenderPass_Vk(VkDevice dev, std::vector<RD_Attachment> attachments, float width, float height);
    ~RD_RenderPass_Vk() override;

    void BeginRenderPass(VkCommandBuffer cmd, VkFramebuffer scFB = VK_NULL_HANDLE);
    void BeginRenderpass(std::shared_ptr<RD_RenderSynchronizer> sync) override;
    void BeginRenderpassExt(std::shared_ptr<RD_RenderSynchronizer> sync, std::shared_ptr<RD_OrphanFramebuffer> fb) override;

    void EndRenderPass(VkCommandBuffer cmd);
    void EndRenderpass(std::shared_ptr<RD_RenderSynchronizer> sync) override;
    void EndRenderpassEXT(std::shared_ptr<RD_RenderSynchronizer> sync, std::shared_ptr<RD_OrphanFramebuffer> fb) override;

    bool BuildRenderpass(RD_API* api, bool sc) override;
    void SetRenderpassSize(RD_API* api, const int w, const int h) override;

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

    bool IsAttachmentTransparent(int idx) override {
        return m_att_desc[idx].is_transparent;
    }

    bool HasDepth() override {
        return m_hasDepth;
    }

    std::shared_ptr<RD_Texture> GetAttachment(int index) override;

private:
    bool MakeFramebuffer(RD_API* api);

    VkFramebuffer m_fb;
    std::vector<std::shared_ptr<RD_Texture_Vk>> m_imgs;

    VkDevice m_dev;
    VkRenderPass m_renderPass;
    bool m_hasDepth;

    std::vector<VkAttachmentDescription> m_att;
    std::vector<RD_Attachment> m_att_desc;
    std::vector<VkClearValue> m_clean;
    float m_w, m_h;
};

#endif //BUILD_VULKAN

#endif //_RD_RENDERPASS_H__
