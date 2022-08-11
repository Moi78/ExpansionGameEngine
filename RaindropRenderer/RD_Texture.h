#ifndef EXPGE_RD_TEXTURE_H
#define EXPGE_RD_TEXTURE_H

#include <iostream>
#include <string>
#include <memory>
#include <filesystem>

#include "RD_Buffer.h"

class RD_Texture {
public:
    RD_Texture() {};
    virtual ~RD_Texture() {};

    virtual bool LoadTextureFromFile(std::string filePath) = 0;
};

#ifdef BUILD_VULKAN

#include <vulkan/vulkan.hpp>

class RD_Texture_Vk : public RD_Texture {
public:
    RD_Texture_Vk(VkDevice dev, VkPhysicalDevice pdev, VkQueue gfxQueue, VkCommandPool cmdPool);
    virtual ~RD_Texture_Vk();

    bool LoadTextureFromFile(std::string filePath) override;

    VkDescriptorImageInfo GetDescriptorInfo();

private:
    bool CreateImage(int w, int h);
    bool CreateImageView();
    bool CreateImageSampler();
    void TransitionImageLayout(VkCommandBuffer cmdBuff, VkImageLayout from, VkImageLayout to);
    void CopyFromBuffer(VkCommandBuffer cmdBuff,std::unique_ptr<RD_Buffer_Vk>& buffer, uint32_t w, uint32_t h);

    VkImage m_imgHdl;
    VkImageView m_imgView;
    VkSampler m_imgSampler;
    VkDeviceMemory m_imgMem;

    VkDevice m_dev;
    VkPhysicalDevice m_pdev;
    VkQueue m_gfxQueue;
    VkCommandPool m_cmdPool;
};

#endif //BUILD_VULKAN

#endif //EXPGE_RD_TEXTURE_H
