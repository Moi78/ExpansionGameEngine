#ifndef EXPGE_RD_IMAGEFORMAT_H
#define EXPGE_RD_IMAGEFORMAT_H

#define IMGFORMAT_R 1
#define IMGFORMAT_RG 2
#define IMGFORMAT_RGB 3
#define IMGFORMAT_RGBA 4

#define IMGFORMAT_R16F 5
#define IMGFORMAT_RG16F 6
#define IMGFORMAT_RGB16F 7
#define IMGFORMAT_RGBA16F 8

#define IMGFORMAT_DEPTH 9

#define IMGFORMAT_BGRA 10
#define IMGFORMAT_BGR 11

#define IMGFORMAT_R32F 12
#define IMGFORMAT_RG32F 13
#define IMGFORMAT_RGB32F 14
#define IMGFORMAT_RGBA32F 15

#ifdef BUILD_VULKAN

#include <vulkan/vulkan.hpp>
#include <unordered_map>

VkFormat GetVKFormat(int format);

#endif //BUILD_VULKAN

#endif //EXPGE_RD_IMAGEFORMAT_H
