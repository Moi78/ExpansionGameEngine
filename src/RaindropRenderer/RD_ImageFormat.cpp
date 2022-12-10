#include "RD_ImageFormat.h"

#ifdef BUILD_VULKAN

VkFormat GetVKFormat(int format) {
    std::unordered_map<int, VkFormat> map_formats {
            {IMGFORMAT_R, VK_FORMAT_R8_SRGB},
            {IMGFORMAT_RG, VK_FORMAT_R8G8_SRGB},
            {IMGFORMAT_RGB, VK_FORMAT_R8G8B8_SRGB},
            {IMGFORMAT_RGBA, VK_FORMAT_R8G8B8A8_SRGB},

            {IMGFORMAT_R16F, VK_FORMAT_R16_SFLOAT},
            {IMGFORMAT_RG16F, VK_FORMAT_R16G16_SFLOAT},
            {IMGFORMAT_RGB16F, VK_FORMAT_R16G16B16_SFLOAT},
            {IMGFORMAT_RGBA16F, VK_FORMAT_R16G16B16A16_SFLOAT},

            {IMGFORMAT_R32F, VK_FORMAT_R32_SFLOAT},
            {IMGFORMAT_RG32F, VK_FORMAT_R32G32_SFLOAT},
            {IMGFORMAT_RGB32F, VK_FORMAT_R32G32B32_SFLOAT},
            {IMGFORMAT_RGBA32F, VK_FORMAT_R32G32B32A32_SFLOAT},

            {IMGFORMAT_DEPTH, VK_FORMAT_D32_SFLOAT},

            {IMGFORMAT_BGRA, VK_FORMAT_B8G8R8A8_UNORM},
            {IMGFORMAT_BGR, VK_FORMAT_B8G8R8_UNORM}
    };

    return map_formats[format];
}

size_t GetVKFormatSize(int format) {
    std::unordered_map<int, size_t> map_sizes {
            {IMGFORMAT_R, sizeof(uint8_t)},
            {IMGFORMAT_RG, 2 * sizeof(uint8_t)},
            {IMGFORMAT_RGB, 3 * sizeof(uint8_t)},
            {IMGFORMAT_RGBA, 4 * sizeof(uint8_t)},

            {IMGFORMAT_R16F, sizeof(float)},
            {IMGFORMAT_RG16F, 2 * sizeof(float) / 2},
            {IMGFORMAT_RGB16F, 3 * sizeof(float) / 2},
            {IMGFORMAT_RGBA16F, 4 * sizeof(float) / 2},

            {IMGFORMAT_R32F, sizeof(float)},
            {IMGFORMAT_RG32F, 2 * sizeof(float)},
            {IMGFORMAT_RGB32F, 3 * sizeof(float)},
            {IMGFORMAT_RGBA32F, 4 * sizeof(float)},

            {IMGFORMAT_DEPTH, sizeof(float)},

            {IMGFORMAT_BGRA, 4 * sizeof(uint8_t)},
            {IMGFORMAT_BGR, 3 * sizeof(uint8_t)}
    };
}

#endif //BUILD_VULKAN