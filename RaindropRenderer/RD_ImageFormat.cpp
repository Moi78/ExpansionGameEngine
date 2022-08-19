#include "RD_ImageFormat.h"

#ifdef BUILD_VULKAN

VkFormat GetVKFormat(int format) {
    std::map<int, VkFormat> map_formats {
            {IMGFORMAT_R, VK_FORMAT_R8_SRGB},
            {IMGFORMAT_RG, VK_FORMAT_R8G8_SRGB},
            {IMGFORMAT_RGB, VK_FORMAT_R8G8B8_SRGB},
            {IMGFORMAT_RGBA, VK_FORMAT_R8G8B8A8_SRGB},

            {IMGFORMAT_R16F, VK_FORMAT_R32_SFLOAT},
            {IMGFORMAT_RG16F, VK_FORMAT_R32G32_SFLOAT},
            {IMGFORMAT_RGB16F, VK_FORMAT_R32G32B32_SFLOAT},
            {IMGFORMAT_RGBA16F, VK_FORMAT_R32G32B32A32_SFLOAT},

            {IMGFORMAT_DEPTH, VK_FORMAT_D32_SFLOAT},

            {IMGFORMAT_BGRA, VK_FORMAT_B8G8R8A8_UNORM},
            {IMGFORMAT_BGR, VK_FORMAT_B8G8R8_UNORM}
    };

    return map_formats[format];
}

#endif //BUILD_VULKAN