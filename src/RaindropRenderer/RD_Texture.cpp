#include "RD_Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

RD_Texture_Vk::RD_Texture_Vk(VkDevice dev, VkPhysicalDevice pdev, VkQueue gfxQueue, VkCommandPool cmdPool) {
    m_dev = dev;
    m_pdev = pdev;
    m_gfxQueue = gfxQueue;
    m_cmdPool = cmdPool;

    m_isDepth = false;
}

RD_Texture_Vk::~RD_Texture_Vk() {
    vkDestroySampler(m_dev, m_imgSampler, nullptr);
    vkDestroyImageView(m_dev, m_imgView, nullptr);
    vkDestroyImage(m_dev, m_imgHdl, nullptr);
    vkFreeMemory(m_dev, m_imgMem, nullptr);
}

bool RD_Texture_Vk::LoadTextureFromFile(std::string filePath) {
    uint8_t defaultData[] = {0, 0, 0, 255, 255, 0, 255, 255, 255, 0, 255, 255, 0, 0, 0, 255};
    if(!std::filesystem::exists(filePath)) {
        std::cerr << "ERROR: File " << filePath << " does not exist." << std::endl;

        CreateTextureFromData(IMGFORMAT_RGBA, 2, 2, defaultData);

        return false;
    }

    int tx_w, tx_h, tx_chans = 0;
    uint8_t* pixels = stbi_load(filePath.c_str(), &tx_w, &tx_h, &tx_chans, STBI_rgb_alpha);

    if(pixels == nullptr) {
        std::cerr << "ERROR: Failed to load texture stored at " << filePath << std::endl;

        CreateTextureFromData(IMGFORMAT_RGBA, 2, 2, defaultData);

        return false;
    }

    size_t buffSize = tx_w * tx_h * 4;
    std::unique_ptr<RD_Buffer_Vk> stagingBuff = std::make_unique<RD_Buffer_Vk>(m_dev, m_pdev, m_gfxQueue, m_cmdPool);

    stagingBuff->BuildAndAllocateBuffer(
            buffSize, RD_BufferUsage::BUFF_TRANSFER_SRC,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );
    stagingBuff->FillBufferData(pixels);

    stbi_image_free(pixels);

    if(!CreateImage(GetVKFormat(IMGFORMAT_RGBA), tx_w, tx_h)) {
        std::cerr << "ERROR: Failed to create image." << std::endl;
        return false;
    }

    auto cmdBuffer = BeginOneTimeCommand(m_dev, m_cmdPool);
    TransitionImageLayout(cmdBuffer, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    CopyFromBuffer(cmdBuffer, stagingBuff, tx_w, tx_h);
    TransitionImageLayout(cmdBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    EndOneTimeCommand(m_dev, m_cmdPool, cmdBuffer, m_gfxQueue);

    if(!CreateImageView(VK_FORMAT_R8G8B8A8_UNORM)) {
        return false;
    }

    if(!CreateImageSampler()) {
        return false;
    }

    return true;
}

bool RD_Texture_Vk::CreateTextureFromData(int format, int w, int h, void* data) {
    VkFormat fmt = GetVKFormat(format);
    size_t fmt_size = GetFormatSize(format);

    if(!CreateImage(fmt, w, h, false)) {
        return false;
    }

    size_t buffSize = w * h * fmt_size;

    std::unique_ptr<RD_Buffer_Vk> stagingBuff = std::make_unique<RD_Buffer_Vk>(m_dev, m_pdev, m_gfxQueue, m_cmdPool);

    stagingBuff->BuildAndAllocateBuffer(
            buffSize, RD_BufferUsage::BUFF_TRANSFER_SRC,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );
    stagingBuff->FillBufferData(data);

    auto cmdBuffer = BeginOneTimeCommand(m_dev, m_cmdPool);
    TransitionImageLayout(cmdBuffer, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    CopyFromBuffer(cmdBuffer, stagingBuff, w, h);
    TransitionImageLayout(cmdBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    EndOneTimeCommand(m_dev, m_cmdPool, cmdBuffer, m_gfxQueue);

    if(!CreateImageView(fmt)) {
        return false;
    }

    if(!CreateImageSampler()) {
        return false;
    }

    return true;
}

bool RD_Texture_Vk::CreateTextureFBReady(int format, int w, int h) {
    VkFormat fmt = GetVKFormat(format);

    if(!CreateImage(fmt, w, h, true)) {
        return false;
    }

    auto cmdBuffer = BeginOneTimeCommand(m_dev, m_cmdPool);
    bool hasDepth = fmt == VK_FORMAT_D32_SFLOAT;

    if(fmt == VK_FORMAT_D32_SFLOAT) {
        TransitionImageLayout(cmdBuffer, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        TransitionImageLayout(cmdBuffer, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        m_isDepth = true;
    } else {
        TransitionImageLayout(cmdBuffer, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    }
    EndOneTimeCommand(m_dev, m_cmdPool, cmdBuffer, m_gfxQueue);

    if(!CreateImageView(fmt, hasDepth)) {
        return false;
    }

    if(!CreateImageSampler()) {
        return false;
    }

    return true;
}

void RD_Texture_Vk::PrepareForRendering(VkCommandBuffer cmdBuff) {
    if(!m_isDepth) {
        TransitionImageLayout(cmdBuff, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                              VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    } else {
        TransitionImageLayout(cmdBuff, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                              VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
    }
}

void RD_Texture_Vk::PrepareForSampling(VkCommandBuffer cmdBuff) {
    if(!m_isDepth) {
        TransitionImageLayout(cmdBuff, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                              VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    } else {
        TransitionImageLayout(cmdBuff, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                              VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    }
}

bool RD_Texture_Vk::CreateImage(VkFormat fmt, int w, int h, bool inFB) {
    VkImageCreateInfo cInfo{};
    cInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    cInfo.imageType = VK_IMAGE_TYPE_2D;
    cInfo.extent = { static_cast<uint32_t>(w), static_cast<uint32_t>(h), 1 };
    cInfo.mipLevels = 1;
    cInfo.arrayLayers = 1;
    cInfo.format = fmt;
    cInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    cInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    cInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    cInfo.samples = VK_SAMPLE_COUNT_1_BIT;

    if(inFB) {
        if(fmt == VK_FORMAT_D32_SFLOAT) {
            cInfo.usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        } else {
            cInfo.usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        }
    }

    if(vkCreateImage(m_dev, &cInfo, nullptr, &m_imgHdl) != VK_SUCCESS) {
        std::cerr << "ERROR: Failed to create image." << std::endl;
        return false;
    }

    VkMemoryRequirements req;
    vkGetImageMemoryRequirements(m_dev, m_imgHdl, &req);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = req.size;
    allocInfo.memoryTypeIndex = FindMemoryType(m_pdev, req.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    if(vkAllocateMemory(m_dev, &allocInfo, nullptr, &m_imgMem) != VK_SUCCESS) {
        std::cerr << "ERROR: Failed to allocate GPU memory for an image." << std::endl;
        return false;
    }

    vkBindImageMemory(m_dev, m_imgHdl, m_imgMem, 0);

    return true;
}

void RD_Texture_Vk::TransitionImageLayout(VkCommandBuffer cmdBuff, VkImageLayout from, VkImageLayout to) {
    VkImageMemoryBarrier memBarrier{};
    memBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    memBarrier.oldLayout = from;
    memBarrier.newLayout = to;
    memBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    memBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    memBarrier.image = m_imgHdl;
    memBarrier.subresourceRange.aspectMask = !m_isDepth ? VK_IMAGE_ASPECT_COLOR_BIT : VK_IMAGE_ASPECT_DEPTH_BIT;
    memBarrier.subresourceRange.layerCount = 1;
    memBarrier.subresourceRange.levelCount = 1;
    memBarrier.subresourceRange.baseArrayLayer = 0;
    memBarrier.subresourceRange.baseMipLevel = 0;

    VkPipelineStageFlags srcFlags;
    VkPipelineStageFlags dstFlags;
    if((from == VK_IMAGE_LAYOUT_UNDEFINED) && (to == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)) {
        memBarrier.srcAccessMask = 0;
        memBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        srcFlags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        dstFlags = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } else if((from == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) && (to == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)) {
        memBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        memBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        srcFlags = VK_PIPELINE_STAGE_TRANSFER_BIT;
        dstFlags = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else if((from == VK_IMAGE_LAYOUT_UNDEFINED) && (to == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)) {
        memBarrier.srcAccessMask = 0;
        memBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        srcFlags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        dstFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    } else if((from == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) && (to == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)) {
        memBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        memBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        srcFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dstFlags = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else if((from == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) && (to == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)) {
        memBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
        memBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        srcFlags = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        dstFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    } else if((from == VK_IMAGE_LAYOUT_UNDEFINED) && (to == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)) {
        memBarrier.srcAccessMask = 0;
        memBarrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
        memBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

        srcFlags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        dstFlags = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;

    } else if((from == VK_IMAGE_LAYOUT_UNDEFINED) && (to == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)) {
        memBarrier.srcAccessMask = 0;
        memBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        srcFlags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        dstFlags = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else if((from == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) && (to == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)) {
        memBarrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        memBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        memBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

        srcFlags = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dstFlags = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else if((from == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) && (to == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)) {
        memBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
        memBarrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        memBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

        srcFlags = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        dstFlags = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    } else {
            std::cerr << "ERROR: Unsupported image transition." << std::endl;
            return;
    }


    vkCmdPipelineBarrier(cmdBuff, srcFlags, dstFlags, 0, 0, nullptr, 0, nullptr, 1, &memBarrier);
}

void RD_Texture_Vk::CopyFromBuffer(VkCommandBuffer cmdBuff, std::unique_ptr<RD_Buffer_Vk> &buffer, uint32_t w, uint32_t h) {
    VkBufferImageCopy copyInfo{};
    copyInfo.bufferOffset = 0;
    copyInfo.bufferRowLength = 0;
    copyInfo.bufferImageHeight = 0;

    copyInfo.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    copyInfo.imageSubresource.baseArrayLayer = 0;
    copyInfo.imageSubresource.layerCount = 1;
    copyInfo.imageSubresource.mipLevel = 0;

    copyInfo.imageExtent = {
        w,
        h,
        1
    };
    copyInfo.imageOffset = {0};

    vkCmdCopyBufferToImage(
            cmdBuff,
            buffer->GetBufferHandle(),
            m_imgHdl,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &copyInfo
    );
}

bool RD_Texture_Vk::CreateImageView(VkFormat fmt, bool depthAtt) {
    VkImageViewCreateInfo cInfo{};
    cInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    cInfo.image = m_imgHdl;
    cInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    cInfo.format = fmt;
    cInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    cInfo.subresourceRange.layerCount = 1;
    cInfo.subresourceRange.baseArrayLayer = 0;
    cInfo.subresourceRange.baseMipLevel = 0;
    cInfo.subresourceRange.levelCount = 1;

    if(depthAtt) {
        cInfo.format = VK_FORMAT_D32_SFLOAT;
        cInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    }

    if(vkCreateImageView(m_dev, &cInfo, nullptr, &m_imgView) != VK_SUCCESS) {
        std::cerr << "ERROR: Failed to create image view." << std::endl;
        return false;
    }

    return true;
}

bool RD_Texture_Vk::CreateImageSampler() {
    VkSamplerCreateInfo cInfo{};
    cInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    cInfo.magFilter = VK_FILTER_LINEAR;
    cInfo.minFilter = VK_FILTER_NEAREST;
    cInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    cInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    cInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

    VkPhysicalDeviceProperties prop;
    vkGetPhysicalDeviceProperties(m_pdev, &prop);

    cInfo.anisotropyEnable = VK_TRUE;
    cInfo.maxAnisotropy = prop.limits.maxSamplerAnisotropy;

    cInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    cInfo.unnormalizedCoordinates = VK_FALSE;
    cInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    cInfo.mipLodBias = 0.0f;
    cInfo.minLod = 0.0f;
    cInfo.maxLod = 0.0f;

    if(vkCreateSampler(m_dev, &cInfo, nullptr, &m_imgSampler) != VK_SUCCESS) {
        std::cerr << "ERROR: Failed to create image sampler." << std::endl;
        return false;
    }

    return true;
}

VkDescriptorImageInfo RD_Texture_Vk::GetDescriptorInfo() {
    VkDescriptorImageInfo info;
    info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    info.imageView = GetView();
    info.sampler = m_imgSampler;

    return info;
}

VkImageView RD_Texture_Vk::GetView() {
    assert(m_imgView != VK_NULL_HANDLE);
    return m_imgView;
}