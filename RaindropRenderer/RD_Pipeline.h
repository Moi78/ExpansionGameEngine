#ifndef _RD_PIPELINE_H__
#define _RD_PIPELINE_H__

#include "RD_RenderPass.h"
#include "RD_ShaderLoader.h"
#include "RD_VertexBuffer.h"

#include <iostream>
#include <memory>

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
};

#ifdef BUILD_VULKAN

#include <vulkan/vulkan.h>

class RD_Pipeline_Vk : public RD_Pipeline {
public:
    RD_Pipeline_Vk(VkDevice dev, VkCommandPool pool, std::shared_ptr<RD_RenderPass> rpass, std::shared_ptr<RD_ShaderLoader> shader);
    ~RD_Pipeline_Vk() override;
    void CleanUp();

    void Bind() override;
    void Unbind() override;

    void BindSC(VkFramebuffer fb);
    void SubmitCMD(VkQueue queue, VkSemaphore waitSignal, VkSemaphore rndrFinished, VkFence inFlight);

    bool BuildPipeline() override;
    void RebuildPipeline() override;

    void DrawVertexBuffer(std::shared_ptr<RD_VertexBuffer> vbuff) override;
    void DrawIndexedVertexBuffer(std::shared_ptr<RD_IndexedVertexBuffer> vibuff) override;

private:
    bool AllocCMDBuffer();

    VkDevice m_dev;
    VkCommandPool m_pool;

    std::shared_ptr<RD_RenderPass_Vk> m_rpass;
    std::shared_ptr<RD_ShaderLoader_Vk> m_shader;

    VkPipelineLayout m_layout;
    VkPipeline m_pipeline;

    VkCommandBuffer m_cmdBuffer;
};

#endif //BUILD_VULKAN

#endif //_RD_PIPELINE_H__
