#ifndef EXPGE_RD_BUFFER_H
#define EXPGE_RD_BUFFER_H

#include <vector>
#include <map>
#include <iostream>
#include <memory>

enum RD_BufferUsage {
    BUFF_VERTEX = 1,
    BUFF_TRANSFER_DEST = 2
};

class RD_Buffer {
public:
    RD_Buffer() {};
    virtual ~RD_Buffer() {};

    virtual size_t GetBufferSize() { return 0; };

    virtual bool FillBufferData(void* data) { return false; };
    virtual bool BuildAndAllocateBuffer(size_t size, RD_BufferUsage usage, int flags) { return false; };

    virtual bool CopyFromBuffer(std::unique_ptr<RD_Buffer>& src) { return false; };
};

#ifdef BUILD_VULKAN

#include <vulkan/vulkan.hpp>

uint32_t FindMemoryType(VkPhysicalDevice pdev, uint32_t filter, VkMemoryPropertyFlags memflags);


class RD_Buffer_Vk : public RD_Buffer {
public:
    RD_Buffer_Vk(VkDevice dev, VkPhysicalDevice pdev, VkQueue gfxQueue, VkCommandPool cmdPool) {
        m_dev = dev;
        m_pdev = pdev;

        m_gfxQueue = gfxQueue;
        m_cmdPool = cmdPool;

        m_buff = VK_NULL_HANDLE;
        m_buffMem = VK_NULL_HANDLE;

        m_bufferSize = 0;
    }

    ~RD_Buffer_Vk() override {
        vkDestroyBuffer(m_dev, m_buff, nullptr);
        vkFreeMemory(m_dev, m_buffMem, nullptr);
    }

    bool BuildAndAllocateBuffer(size_t size, RD_BufferUsage usage, int flags) override {
        std::map<RD_BufferUsage, VkBufferUsageFlags > corresp = {
                {RD_BufferUsage::BUFF_VERTEX, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT},
                {RD_BufferUsage::BUFF_TRANSFER_DEST, VK_BUFFER_USAGE_TRANSFER_DST_BIT}
        };

        std::initializer_list<RD_BufferUsage> allUsages = {RD_BufferUsage::BUFF_VERTEX,
                                                           RD_BufferUsage::BUFF_TRANSFER_DEST};

        VkBufferUsageFlags useFlag;
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

    bool FillBufferData(void* data) override {
        void* data_;
        vkMapMemory(m_dev, m_buffMem, 0, m_bufferSize, 0, &data_);
        memcpy(data_, data, m_bufferSize);
        vkUnmapMemory(m_dev, m_buffMem);

        return true;
    }

    VkBuffer GetBufferHandle() {
        return m_buff;
    }

    size_t GetBufferSize() override {
        return m_bufferSize;
    }

    bool CopyFromBuffer(std::unique_ptr<RD_Buffer_Vk>& buff) {
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

private:
    bool CreateBuffer(size_t size, VkBufferUsageFlags usage) {
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

    VkDevice m_dev;
    VkPhysicalDevice m_pdev;

    VkCommandPool m_cmdPool;
    VkQueue m_gfxQueue;

    VkBuffer m_buff;
    VkDeviceMemory m_buffMem;

    size_t m_bufferSize;
};

#endif

#endif //EXPGE_RD_BUFFER_H
