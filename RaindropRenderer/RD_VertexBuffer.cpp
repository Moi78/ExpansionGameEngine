#include "RD_VertexBuffer.h"

std::vector<float> MakeVertexData(std::vector<vec3>& pos, std::vector<vec3>& norm, std::vector<vec2>& uv) {
    assert((pos.size() == norm.size()) && (norm.size() == uv.size()) && "Every vector must have the same size");

    std::vector<float> res(pos.size() + norm.size() + uv.size());
    for(int i = 0; i < pos.size(); i++) {
        std::copy(&pos[i].GetData()[0], &pos[i].GetData()[2], res.begin() + (i * 8));
        std::copy(&norm[i].GetData()[0], &norm[i].GetData()[2], res.begin() + ((i + 3) * 8));
        std::copy(&uv[i].GetData()[0], &uv[i].GetData()[1], res.begin() + ((i + 6) * 8));
    }

    return res;
}

#ifdef BUILD_VULKAN

uint32_t FindMemoryType(VkPhysicalDevice pdev, uint32_t filter, VkMemoryPropertyFlags memflags) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(pdev, &memProperties);

    for(uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if((filter & (1 << i)) && ((memProperties.memoryTypes[i].propertyFlags & memflags) == memflags)) {
            return i;
        }
    }

    throw std::runtime_error("ERROR: Failed to find suitable memory type.");
}

RD_VertexBuffer_Vk::RD_VertexBuffer_Vk(VkDevice dev, VkPhysicalDevice pdev) {
    m_dev = dev;
    m_pdev = pdev;
}

RD_VertexBuffer_Vk::~RD_VertexBuffer_Vk() {
    vkDestroyBuffer(m_dev, m_vBuff, nullptr);
    vkFreeMemory(m_dev, m_vBuffMem, nullptr);
}

bool RD_VertexBuffer_Vk::FillBufferData(std::vector<float>& vertexData) {
    if(CreateBuffer(vertexData.size())) {
        std::cerr << "ERROR: Failed to create a vertex buffer." << std::endl;
        return false;
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(m_dev, m_vBuff, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(m_pdev, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if(vkAllocateMemory(m_dev, &allocInfo, nullptr, &m_vBuffMem) != VK_SUCCESS) {
        std::cerr << "ERROR: Failed to allocate vertex buffer memory." << std::endl;
        return false;
    }

    void* data;
    vkMapMemory(m_dev, m_vBuffMem, 0, vertexData.size(), 0, &data);
    std::move(vertexData.begin(), vertexData.end(), (float*)data);
    vkUnmapMemory(m_dev, m_vBuffMem);

    m_buffSize = vertexData.size();
}

bool RD_VertexBuffer_Vk::CreateBuffer(size_t nbrVerts) {
    VkBufferCreateInfo buffInfo{};
    buffInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffInfo.size = nbrVerts;
    buffInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    buffInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if(vkCreateBuffer(m_dev, &buffInfo, nullptr, &m_vBuff) != VK_SUCCESS) {
        return false;
    }

    return true;
}

size_t RD_VertexBuffer_Vk::GetBufferSize() {
    return m_buffSize;
}

void RD_VertexBuffer_Vk::BindBuffer(VkCommandBuffer cmd) {
    VkDeviceSize offset = {0};

    vkCmdBindVertexBuffers(cmd, 0, 1, &m_vBuff, &offset);
}

#endif //BUILD_VULKAN