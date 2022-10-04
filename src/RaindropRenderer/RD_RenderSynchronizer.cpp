#include "RD_RenderSynchronizer.h"

#ifdef BUILD_VULKAN

RD_RenderSynchronizer_Vk::RD_RenderSynchronizer_Vk(VkDevice dev, VkCommandPool pool, VkQueue gfxQueue) {
    m_pool = pool;
    m_dev = dev;
    m_gfxQueue = gfxQueue;

    m_cmdBuffer = VK_NULL_HANDLE;
    m_fence = VK_NULL_HANDLE;

    Init();
}

RD_RenderSynchronizer_Vk::~RD_RenderSynchronizer_Vk() {
    vkDestroyFence(m_dev, m_fence, nullptr);
}

void RD_RenderSynchronizer_Vk::Init() {
    VkCommandBufferAllocateInfo aInfo{};
    aInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    aInfo.commandBufferCount = 1;
    aInfo.commandPool = m_pool;
    aInfo.pNext = nullptr;

    if(vkAllocateCommandBuffers(m_dev, &aInfo, &m_cmdBuffer) != VK_SUCCESS) {
        std::cerr << "ERROR: Failed to allocate command buffer." << std::endl;
    }

    // Sync object creation
    VkFenceCreateInfo cInfo{};
    cInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    cInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    if(vkCreateFence(m_dev, &cInfo, nullptr, &m_fence) != VK_SUCCESS) {
        std::cerr << "ERROR: Failed to create fence." << std::endl;
    }
}

VkCommandBuffer RD_RenderSynchronizer_Vk::GetCommandBuffer() {
    return m_cmdBuffer;
}

void RD_RenderSynchronizer_Vk::Start() {
    vkResetCommandBuffer(m_cmdBuffer, 0);

    VkCommandBufferBeginInfo bInfo{};
    bInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    vkBeginCommandBuffer(m_cmdBuffer, &bInfo);
}

void RD_RenderSynchronizer_Vk::Stop() {
    vkEndCommandBuffer(m_cmdBuffer);

    vkResetFences(m_dev, 1, &m_fence);

    VkSubmitInfo sbInfo{};
    sbInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    sbInfo.waitSemaphoreCount = 0;
    sbInfo.signalSemaphoreCount = 0;

    sbInfo.commandBufferCount = 1;
    sbInfo.pCommandBuffers = &m_cmdBuffer;

    if(vkQueueSubmit(m_gfxQueue, 1, &sbInfo, m_fence) != VK_SUCCESS) {
        std::cerr << "ERROR: Failed to submit queue (sync)." << std::endl;
    }

    vkWaitForFences(m_dev, 1, &m_fence, VK_TRUE, UINT64_MAX);
}

#endif //BUILD_VULKAN