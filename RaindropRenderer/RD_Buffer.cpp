#include "RD_Buffer.h"

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

//-------------------------------------------------------------------------

RD_Buffer_Vk::RD_Buffer_Vk(VkDevice dev, VkPhysicalDevice pdev, VkQueue gfxQueue, VkCommandPool cmdPool) {
    m_dev = dev;
    m_pdev = pdev;

    m_gfxQueue = gfxQueue;
    m_cmdPool = cmdPool;

    m_buff = VK_NULL_HANDLE;
    m_buffMem = VK_NULL_HANDLE;

    m_bufferSize = 0;
    }

RD_Buffer_Vk::~RD_Buffer_Vk() {
    vkDestroyBuffer(m_dev, m_buff, nullptr);
    vkFreeMemory(m_dev, m_buffMem, nullptr);
}

bool RD_Buffer_Vk::BuildAndAllocateBuffer(size_t size, RD_BufferUsage usage, int flags) {
    std::map<RD_BufferUsage, VkBufferUsageFlags > corresp = {
            {RD_BufferUsage::BUFF_VERTEX, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT},
            {RD_BufferUsage::BUFF_TRANSFER_DEST, VK_BUFFER_USAGE_TRANSFER_DST_BIT},
            {RD_BufferUsage::BUFF_TRANSFER_SRC, VK_BUFFER_USAGE_TRANSFER_SRC_BIT},
            {RD_BufferUsage::BUFF_INDEX, VK_BUFFER_USAGE_INDEX_BUFFER_BIT},
            {RD_BufferUsage::BUFF_UNIFORM, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT}
    };

    std::initializer_list<RD_BufferUsage> allUsages = {
            RD_BufferUsage::BUFF_VERTEX,
            RD_BufferUsage::BUFF_TRANSFER_DEST,
            RD_BufferUsage::BUFF_TRANSFER_SRC,
            RD_BufferUsage::BUFF_INDEX,
            RD_BufferUsage::BUFF_UNIFORM
    };

    VkBufferUsageFlags useFlag = 0;
    for(auto& f : allUsages) {
        if((f | usage) == usage) {
            useFlag |= corresp[f];
        }
    }

    if(!CreateBuffer(size, useFlag)) {
        std::cout << "ERROR: Failed to create buffer." << std::endl;
        return false;
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(m_dev, m_buff, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(m_pdev, memRequirements.memoryTypeBits, flags);

    if(vkAllocateMemory(m_dev, &allocInfo, nullptr, &m_buffMem) != VK_SUCCESS) {
        std::cerr << "ERROR: Failed to allocate vertex buffer memory." << std::endl;
        return false;
    }

    m_bufferSize = size;

    vkBindBufferMemory(m_dev, m_buff, m_buffMem, 0);

    return true;
}

bool RD_Buffer_Vk::FillBufferData(void* data) {
    void* data_;
    vkMapMemory(m_dev, m_buffMem, 0, m_bufferSize, 0, &data_);
    memcpy(data_, data, m_bufferSize);
    vkUnmapMemory(m_dev, m_buffMem);

    return true;
}

VkBuffer RD_Buffer_Vk::GetBufferHandle() {
    return m_buff;
}

size_t RD_Buffer_Vk::GetBufferSize() {
    return m_bufferSize;
}

bool RD_Buffer_Vk::CopyFromBuffer(std::unique_ptr<RD_Buffer_Vk>& buff) {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = m_cmdPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer cmdBuffer;
    vkAllocateCommandBuffers(m_dev, &allocInfo, &cmdBuffer);

    VkCommandBufferBeginInfo begInfo{};
    begInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(cmdBuffer, &begInfo);

    VkBufferCopy region{};
    region.size = m_bufferSize;

    vkCmdCopyBuffer(cmdBuffer, buff->GetBufferHandle(), m_buff, 1, &region);

    vkEndCommandBuffer(cmdBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmdBuffer;

    vkQueueSubmit(m_gfxQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(m_gfxQueue);

    vkFreeCommandBuffers(m_dev, m_cmdPool, 1, &cmdBuffer);

    return true;
}

bool RD_Buffer_Vk::CreateBuffer(size_t size, VkBufferUsageFlags usage) {
    VkBufferCreateInfo buffInfo{};
    buffInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffInfo.size = size;
    buffInfo.usage = usage;
    buffInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    m_bufferSize = size;

    if(vkCreateBuffer(m_dev, &buffInfo, nullptr, &m_buff) != VK_SUCCESS) {
        return false;
    }

    return true;
}