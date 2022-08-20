#include "RD_UniformBuffer.h"

RD_UniformBuffer_Vk::RD_UniformBuffer_Vk(VkDevice dev, VkPhysicalDevice pdev, uint32_t binding)
        : RD_UniformBuffer(binding) {
    m_buffer = std::make_unique<RD_Buffer_Vk>(dev, pdev, VK_NULL_HANDLE, VK_NULL_HANDLE);
}

RD_UniformBuffer_Vk::~RD_UniformBuffer_Vk() {
    m_buffer.reset();
}

bool RD_UniformBuffer_Vk::BuildAndAllocateBuffer(size_t size) {
    if(!m_buffer->BuildAndAllocateBuffer(size, RD_BufferUsage::BUFF_UNIFORM, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
        std::cerr << "ERROR: Failed to create uniform buffer (binding = " << (int)m_binding << std::endl;
        return false;
    }

    return true;
}

bool RD_UniformBuffer_Vk::FillBufferData(void *data) {
    return m_buffer->FillBufferData(data);
}

bool RD_UniformBuffer_Vk::PartialFillBufferData(void *data, size_t size, size_t offset) {
    return m_buffer->PartialFillBufferData(data, size, offset);
}

VkBuffer RD_UniformBuffer_Vk::GetBufferHandle() {
    return m_buffer->GetBufferHandle();
}

size_t RD_UniformBuffer_Vk::GetBufferSize() {
    return m_buffer->GetBufferSize();
}