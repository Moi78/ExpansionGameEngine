#ifndef _RD_PIPELINE_H__
#define _RD_PIPELINE_H__

#include "RD_RenderPass.h"
#include "RD_ShaderLoader.h"
#include "RD_VertexBuffer.h"
#include "RD_UniformBuffer.h"
#include "RD_Texture.h"
#include "RD_RenderSynchronizer.h"

#include <iostream>
#include <memory>
#include <array>
#include <optional>

enum class RD_CullMode {
    CM_BACK = 0,
    CM_NONE = 1
};

class RD_Pipeline {
public:
    RD_Pipeline() {};
    virtual ~RD_Pipeline() {};

    virtual void Bind(std::optional<std::shared_ptr<RD_RenderSynchronizer>> sync) = 0;
    virtual void Unbind(std::optional<std::shared_ptr<RD_RenderSynchronizer>> sync) = 0;

    virtual void RebuildPipeline() = 0;
    virtual bool BuildPipeline() = 0;

    virtual void DrawVertexBuffer(std::shared_ptr<RD_VertexBuffer> vbuff) = 0;
    virtual void DrawIndexedVertexBuffer(std::shared_ptr<RD_IndexedVertexBuffer> vibuff, std::optional<std::shared_ptr<RD_RenderSynchronizer>> sync) = 0;

    virtual void RegisterUniformBuffer(std::shared_ptr<RD_UniformBuffer> buff) = 0;
    virtual void RegisterTexture(std::shared_ptr<RD_Texture> tex, uint32_t binding) = 0;
    virtual void RegisterTextureArray(std::vector<std::shared_ptr<RD_Texture>> texs, uint32_t binding) = 0;
    virtual void PurgeTextures() = 0;

    virtual void SetCullMode(RD_CullMode cm) = 0;

    virtual void SetTextureArray(std::vector<std::shared_ptr<RD_Texture>> texs, uint32_t binding) = 0;
    virtual void SetTexture(std::shared_ptr<RD_Texture> tex, uint32_t binding) = 0;

    virtual void ConfigurePushConstant(size_t size) = 0;
    virtual void PushConstant(void* data, size_t size, std::optional<std::shared_ptr<RD_RenderSynchronizer>> sync) = 0;
    virtual void PartialPushConstant(void* data, size_t size, size_t offest, std::optional<std::shared_ptr<RD_RenderSynchronizer>> sync) = 0;

    virtual bool IsBuilt() = 0;
    virtual void EnableTransparency() = 0;

    virtual void SwapShader(std::shared_ptr<RD_ShaderLoader> newShader) = 0;
};

#ifdef BUILD_VULKAN

#include <vulkan/vulkan.h>

class RD_Pipeline_Vk : public RD_Pipeline {
public:
    RD_Pipeline_Vk(VkDevice dev, VkCommandPool pool, VkQueue gfxQueue,std::shared_ptr<RD_RenderPass> rpass, std::shared_ptr<RD_ShaderLoader> shader, bool extSignaling = false);
    ~RD_Pipeline_Vk() override;
    void CleanUp();

    void Bind(std::optional<std::shared_ptr<RD_RenderSynchronizer>> sync) override;
    void Unbind(std::optional<std::shared_ptr<RD_RenderSynchronizer>> sync) override;

    void BindSC(VkFramebuffer fb);
    void UnbindSC();

    void SubmitCMD(VkQueue queue, VkSemaphore waitSignal, VkSemaphore rndrFinished, VkFence inFlight);
    void SubmitCMDSync();

    bool BuildPipeline() override;
    void RebuildPipeline() override;

    void DrawVertexBuffer(std::shared_ptr<RD_VertexBuffer> vbuff) override;
    void DrawIndexedVertexBuffer(std::shared_ptr<RD_IndexedVertexBuffer> vibuff, std::optional<std::shared_ptr<RD_RenderSynchronizer>> sync) override;

    void RegisterUniformBuffer(std::shared_ptr<RD_UniformBuffer> buff) override;
    void RegisterTexture(std::shared_ptr<RD_Texture> tex, uint32_t binding) override;
    void RegisterTextureArray(std::vector<std::shared_ptr<RD_Texture>> texs, uint32_t binding) override;
    void PurgeTextures() override;

    void SetCullMode(RD_CullMode cm) override;

    void SetTextureArray(std::vector<std::shared_ptr<RD_Texture>> texs, uint32_t binding) override;
    void SetTexture(std::shared_ptr<RD_Texture> tex, uint32_t binding) override;

    void ConfigurePushConstant(size_t size) override;
    void PushConstant(void* data, size_t size, std::optional<std::shared_ptr<RD_RenderSynchronizer>> sync) override;
    void PartialPushConstant(void* data, size_t size, size_t offset, std::optional<std::shared_ptr<RD_RenderSynchronizer>> sync) override;

    bool IsBuilt() override;
    void EnableTransparency() override;

    void SwapShader(std::shared_ptr<RD_ShaderLoader> newShader) override;

    VkDescriptorPool GetDescPool() { return m_descPool; };
private:
    bool AllocCMDBuffer();
    bool CreateDescriptorPool();
    bool CreateDescriptorSet();
    bool BuildSyncObjects();

    void UpdateDescSets();

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

    std::vector<VkDescriptorSetLayoutBinding> m_bindings_tex_array;
    std::vector<std::vector<std::shared_ptr<RD_Texture>>> m_texs_array;

    VkDescriptorSet m_descSet;
    VkDescriptorSetLayout m_descLayout;
    VkDescriptorPool m_descPool;

    VkPipelineLayout m_layout;
    VkPipeline m_pipeline;

    VkCullModeFlags m_cullMode;

    VkCommandBuffer m_cmdBuffer;

    size_t m_push_ctant_size;

    bool m_isTransparent;
};

#endif //BUILD_VULKAN

#endif //_RD_PIPELINE_H__
