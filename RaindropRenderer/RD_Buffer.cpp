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