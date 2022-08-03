#ifndef EXPGE_RD_BUFFER_H
#define EXPGE_RD_BUFFER_H

#include <vector>
#include <map>
#include <iostream>
#include <memory>

enum RD_BufferUsage {
    BUFF_VERTEX = 1,
    BUFF_TRANSFER_DEST = 2,
    BUFF_TRANSFER_SRC = 4,
    BUFF_INDEX = 8
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
    RD_Buffer_Vk(VkDevice dev, VkPhysicalDevice pdev, VkQueue gfxQueue, VkCommandPool cmdPool);
    ~RD_Buffer_Vk() override;

    bool BuildAndAllocateBuffer(size_t size, RD_BufferUsage usage, int flags) override;
    bool FillBufferData(void* data) override;

    VkBuffer GetBufferHandle();
    size_t GetBufferSize() override;

    bool CopyFromBuffer(std::unique_ptr<RD_Buffer_Vk>& buff);

private:
    bool CreateBuffer(size_t size, VkBufferUsageFlags usage);

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
