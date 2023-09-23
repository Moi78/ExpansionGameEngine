#ifndef EXPGE_RD_RENDERSYNCHRONIZER_H
#define EXPGE_RD_RENDERSYNCHRONIZER_H

#include <memory>
#include <iostream>

class RD_RenderSynchronizer {
public:
    RD_RenderSynchronizer() { m_idx = 0; };
    virtual ~RD_RenderSynchronizer() {};

    virtual void Init(int jobCount = 1) = 0;

    virtual void Start() = 0;
    virtual void Stop() = 0;

    void SetJobIndex(int index) {m_idx = index;}

    virtual void StartJobs() = 0;

protected:
    int m_idx;
};

#ifdef BUILD_VULKAN

#include <vulkan/vulkan.hpp>

class RD_RenderSynchronizer_Vk : public RD_RenderSynchronizer {
public:
    RD_RenderSynchronizer_Vk(VkDevice dev, VkCommandPool pool, VkQueue gfxQueue);
    ~RD_RenderSynchronizer_Vk() override;

    void Init(int jobCount) override;

    void Start() override;
    void Stop() override;

    VkCommandBuffer GetCommandBuffer();

    void StartJobs() override;
private:
    VkCommandPool m_pool;
    VkQueue m_gfxQueue;
    VkDevice m_dev;

    std::vector<VkCommandBuffer> m_cmdBuffers;
    VkFence m_fence;
};

#endif //BUILD_VULKAN

#endif //EXPGE_RD_RENDERSYNCHRONIZER_H
