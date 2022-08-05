#ifndef EXPGE_RD_UNIFORMBUFFER_H
#define EXPGE_RD_UNIFORMBUFFER_H

#include "RD_Buffer.h"

#include <iostream>
#include <memory>

class RD_UniformBuffer {
public:
    RD_UniformBuffer(uint32_t binding) : m_binding(binding) {};
    virtual ~RD_UniformBuffer() {};

    virtual bool BuildAndAllocateBuffer(size_t size) = 0;
    virtual bool FillBufferData(void* data) = 0;

    virtual size_t GetBufferSize() = 0;

    virtual uint32_t GetBinding() {
        return m_binding;
    }

protected:
    uint32_t m_binding;
};

#ifdef BUILD_VULKAN

#include <vulkan/vulkan.hpp>

class RD_UniformBuffer_Vk : public RD_UniformBuffer {
public:
    RD_UniformBuffer_Vk(VkDevice dev, VkPhysicalDevice pdev, uint32_t binding);
    ~RD_UniformBuffer_Vk() override;

    bool BuildAndAllocateBuffer(size_t size) override;
    bool FillBufferData(void* data) override;

    size_t GetBufferSize() override;

    VkBuffer GetBufferHandle();

private:
    std::unique_ptr<RD_Buffer_Vk> m_buffer;
};

#endif //BUILD_VULKAN

#endif //EXPGE_RD_UNIFORMBUFFER_H
