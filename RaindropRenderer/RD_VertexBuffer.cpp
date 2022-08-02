#include "RD_VertexBuffer.h"

std::vector<float> MakeVertexData(std::vector<vec3> pos, std::vector<vec3> norm, std::vector<vec2> uv) {
    assert((pos.size() == norm.size()) && (norm.size() == uv.size()) && "Every vector must have the same size");

    std::vector<float> res;
    for(int i = 0; i < pos.size(); i++) {
        res.push_back(pos[i].GetX());
        res.push_back(pos[i].GetY());
        res.push_back(pos[i].GetZ());

        res.push_back(norm[i].GetX());
        res.push_back(norm[i].GetY());
        res.push_back(norm[i].GetZ());

        res.push_back(uv[i].GetX());
        res.push_back(uv[i].GetY());
    }

    return res;
}

#ifdef BUILD_VULKAN

RD_VertexBuffer_Vk::RD_VertexBuffer_Vk(VkDevice dev, VkPhysicalDevice pdev, VkQueue gfxQueue, VkCommandPool cmdPool) {
    m_buffer = std::make_unique<RD_Buffer_Vk>(dev, pdev, gfxQueue, cmdPool);
    m_stagingBuffer = std::make_unique<RD_Buffer_Vk>(dev, pdev, gfxQueue, cmdPool);
}

RD_VertexBuffer_Vk::~RD_VertexBuffer_Vk() {

}

bool RD_VertexBuffer_Vk::FillBufferData(std::vector<float>& vertexData) {
    if(!m_stagingBuffer->BuildAndAllocateBuffer(vertexData.size() * sizeof(float), RD_BufferUsage::BUFF_VERTEX, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
        std::cerr << "ERROR: Failed to allocate vertex buffer." << std::endl;
        return false;
    }

    m_stagingBuffer->FillBufferData((void*)(vertexData.data()));

    if(!m_buffer->BuildAndAllocateBuffer(vertexData.size() * sizeof(float),
                                         static_cast<RD_BufferUsage>(
                                                 RD_BufferUsage::BUFF_TRANSFER_DEST |
                                                 RD_BufferUsage::BUFF_VERTEX
                                                 ),
                                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
    ) {
        std::cerr << "ERROR: Failed to allocate staging buffer." << std::endl;

        m_stagingBuffer.reset();
        return false;
    }
    if(!m_buffer->CopyFromBuffer(m_stagingBuffer)) {
        std::cerr << "ERROR: Failed to copy from staging buffer to actual buffer." << std::endl;

        return false;
    }


    return true;
}

size_t RD_VertexBuffer_Vk::GetBufferSize() {
    return m_buffer->GetBufferSize();
}

void RD_VertexBuffer_Vk::BindBuffer(VkCommandBuffer cmd) {
    VkDeviceSize offset = {0};

    VkBuffer buff = m_buffer->GetBufferHandle();
    vkCmdBindVertexBuffers(cmd, 0, 1, &buff, &offset);
}

#endif //BUILD_VULKAN