#ifndef EXPGE_RD_RENDERSYNCHRONIZER_H
#define EXPGE_RD_RENDERSYNCHRONIZER_H

#include <memory>
#include <iostream>

class RD_RenderSynchronizer {
public:
    RD_RenderSynchronizer() {};
    virtual ~RD_RenderSynchronizer() {};

    virtual void Start() = 0;
    virtual void Stop() = 0;
};

#ifdef BUILD_VULKAN

#include <vulkan/vulkan.hpp>

class RD_RenderSynchronizer_Vk : public RD_RenderSynchronizer {
public:
    RD_RenderSynchronizer_Vk(VkDevice dev, VkCommandPool pool, VkQueue gfxQueue);
    ~RD_RenderSynchronizer_Vk() override;

    void Start() override;
    void Stop() override;

    VkCommandBuffer GetCommandBuffer();

private:
    void Init();

    VkCommandPool m_pool;
    VkQueue m_gfxQueue;
    VkDevice m_dev;

    VkCommandBuffer m_cmdBuffer;
    VkFence m_fence;
};

#endif //BUILD_VULKAN

#endif //EXPGE_RD_RENDERSYNCHRONIZER_H
