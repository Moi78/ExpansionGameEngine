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
    auto flags_ = static_cast<RD_BufferUsage>(RD_BufferUsage::BUFF_TRANSFER_SRC | RD_BufferUsage::BUFF_VERTEX);
    if(!m_stagingBuffer->BuildAndAllocateBuffer(vertexData.size() * sizeof(float), flags_, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
        std::cerr << "ERROR: Failed to allocate vertex buffer." << std::endl;
        return false;
    }

    m_stagingBuffer->FillBufferData((void*)(vertexData.data()));

    auto flags = static_cast<RD_BufferUsage>(RD_BufferUsage::BUFF_TRANSFER_DEST | RD_BufferUsage::BUFF_VERTEX);

    if(!m_buffer->BuildAndAllocateBuffer(vertexData.size() * sizeof(float),
                                        flags,
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

//-----------------------------------------------------

RD_IndexedVertexBuffer_Vk::RD_IndexedVertexBuffer_Vk(
        VkDevice dev, VkPhysicalDevice pdev, VkQueue gfxQueue, VkCommandPool cmdPool
        ) : RD_VertexBuffer_Vk(dev, pdev, gfxQueue, cmdPool) {
    m_indBuffer = std::make_unique<RD_Buffer_Vk>(dev, pdev, gfxQueue, cmdPool);
    m_stagingIndBuffer = std::make_unique<RD_Buffer_Vk>(dev, pdev, gfxQueue, cmdPool);

    m_nbrInd = 0;
}

RD_IndexedVertexBuffer_Vk::~RD_IndexedVertexBuffer_Vk() {

}

bool RD_IndexedVertexBuffer_Vk::FillBufferData(
        std::vector<float> &vertexData, std::vector<uint32_t> indexData
) {
    if(!RD_VertexBuffer_Vk::FillBufferData(vertexData)) {
        return false;
    }

    auto flags_ = static_cast<RD_BufferUsage>(RD_BufferUsage::BUFF_TRANSFER_SRC | RD_BufferUsage::BUFF_INDEX);
    if(!m_stagingIndBuffer->BuildAndAllocateBuffer(indexData.size() * sizeof(uint32_t), flags_, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
        std::cerr << "ERROR: Failed to allocate vertex buffer." << std::endl;
        return false;
    }

    m_stagingIndBuffer->FillBufferData((void*)(indexData.data()));

    auto flags = static_cast<RD_BufferUsage>(RD_BufferUsage::BUFF_TRANSFER_DEST | RD_BufferUsage::BUFF_INDEX);

    if(!m_indBuffer->BuildAndAllocateBuffer(indexData.size() * sizeof(uint32_t),
                                         flags,
                                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
            ) {
        std::cerr << "ERROR: Failed to allocate staging buffer." << std::endl;

        m_stagingIndBuffer.reset();
        return false;
    }
    if(!m_indBuffer->CopyFromBuffer(m_stagingIndBuffer)) {
        std::cerr << "ERROR: Failed to copy from staging buffer to actual buffer." << std::endl;

        return false;
    }

    m_nbrInd = indexData.size();

    return true;
}

size_t RD_IndexedVertexBuffer_Vk::GetBufferSize() {
    return RD_VertexBuffer_Vk::GetBufferSize() + m_indBuffer->GetBufferSize();
}

void RD_IndexedVertexBuffer_Vk::BindBuffer(VkCommandBuffer cmd) {
    RD_VertexBuffer_Vk::BindBuffer(cmd);

    VkDeviceSize offset = {0};
    VkBuffer handle = m_indBuffer->GetBufferHandle();
    vkCmdBindIndexBuffer(cmd, handle, offset, VkIndexType::VK_INDEX_TYPE_UINT32);
}

uint32_t RD_IndexedVertexBuffer_Vk::GetNumberOfIndices() {
    return m_nbrInd;
}

#endif //BUILD_VULKAN