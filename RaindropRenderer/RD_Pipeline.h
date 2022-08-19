#ifndef _RD_PIPELINE_H__
#define _RD_PIPELINE_H__

#include "RD_RenderPass.h"
#include "RD_ShaderLoader.h"
#include "RD_VertexBuffer.h"
#include "RD_UniformBuffer.h"
#include "RD_Texture.h"

#include <iostream>
#include <memory>
#include <array>

class RD_Pipeline {
public:
    RD_Pipeline() {};
    virtual ~RD_Pipeline() {};

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual void RebuildPipeline() = 0;
    virtual bool BuildPipeline() = 0;

    virtual void DrawVertexBuffer(std::shared_ptr<RD_VertexBuffer> vbuff) = 0;
    virtual void DrawIndexedVertexBuffer(std::shared_ptr<RD_IndexedVertexBuffer> vibuff) = 0;

    virtual void RegisterUniformBuffer(std::shared_ptr<RD_UniformBuffer>& buff) = 0;
    virtual void RegisterTexture(std::shared_ptr<RD_Texture>& tex, uint32_t binding) = 0;
};

#ifdef BUILD_VULKAN

#include <vulkan/vulkan.h>

class RD_Pipeline_Vk : public RD_Pipeline {
public:
    RD_Pipeline_Vk(VkDevice dev, VkCommandPool pool, VkQueue gfxQueue,std::shared_ptr<RD_RenderPass> rpass, std::shared_ptr<RD_ShaderLoader> shader, bool extSignaling = false);
    ~RD_Pipeline_Vk() override;
    void CleanUp();

    void Bind() override;
    void Unbind() override;

    void BindSC(VkFramebuffer fb);
    void UnbindSC();

    void SubmitCMD(VkQueue queue, VkSemaphore waitSignal, VkSemaphore rndrFinished, VkFence inFlight);
    void SubmitCMDSync();

    bool BuildPipeline() override;
    void RebuildPipeline() override;

    void DrawVertexBuffer(std::shared_ptr<RD_VertexBuffer> vbuff) override;
    void DrawIndexedVertexBuffer(std::shared_ptr<RD_IndexedVertexBuffer> vibuff) override;

    void RegisterUniformBuffer(std::shared_ptr<RD_UniformBuffer>& buff) override;
    void RegisterTexture(std::shared_ptr<RD_Texture>& tex, uint32_t binding) override;

private:
    bool AllocCMDBuffer();
    bool CreateDescriptorPool();
    bool CreateDescriptorSet();
    bool BuildSyncObjects();

    VkDevice m_dev;
    VkCommandPool m_pool;
    VkQueue m_gfxQueue;

    bool m_extSignaling;
    VkFence m_fence;

    std::shared_ptr<RD_RenderPass_Vk> m_rpass;
    std::shared_ptr<RD_ShaderLoader_Vk> m_shader;

    std::vector<std::shared_ptr<RD_UniformBuffer>> m_uBuffs;
    std::vector<std::shared_ptr<RD_Texture>> m_texs;

    std::vector<VkDescriptorSetLayoutBinding> m_bindings;
    std::vector<VkDescriptorSetLayoutBinding> m_bindings_tex;
    VkDescriptorSet m_descSet;
    VkDescriptorSetLayout m_descLayout;
    VkDescriptorPool m_descPool;

    VkPipelineLayout m_layout;
    VkPipeline m_pipeline;

    VkCommandBuffer m_cmdBuffer;
};

#endif //BUILD_VULKAN

#endif //_RD_PIPELINE_H__
